#include <sstream>

#include "glog/logging.h"

#include "agent_server.h"

AgentServer::AgentServer()
{
    usock_server_ = NULL;
    log_sdk_ = NULL;
    log_data_ = NULL;
    log_common_data_.info_BIZ_type = "user_coloration_log_kafka_agent"; //对应的topic，写死一般不会变动，后期可以改为配置
}

AgentServer::~AgentServer()
{}

void AgentServer::open()
{
    usock_server_ = createUDSDatagramServer();
    if (NULL == usock_server_) {
        throw AgentServerEXception("new UDSDatagramServer failed");
    }

    //uds
    int ret = usock_server_->open(conf.get_uds_file().c_str(), this);   //独立的线程接收，收到后回调
    if (0 != ret) {
        std::stringstream ss;
        ss << "usock_server_ open failed, err code = " << ret;
        throw AgentServerEXception(ss.str());
    }

    //log sdk
    if (!create_LogCollectSDK(&log_sdk_)) {
        std::stringstream ss;
        ss << "create_LogCollectSDK, NULL";
        throw AgentServerEXception(ss.str());
    }
    create_LogData(&log_data_);

    log_collect::RET_NUM ret_log = log_sdk_->open(conf.get_local_ip(), conf.get_log_sdk_host(), conf.get_log_sdk_name(), conf.get_log_sdk_version());
    if (ret_log != log_collect::RET_NUM_SUCCESS) {
        std::stringstream ss;
        ss << "log_sdk_->open, err, ret = " << ret_log;
        throw AgentServerEXception(ss.str());
    }

    LOG(INFO) << "agent server open success";

    return;
}

void AgentServer::onRecv(const char * peer_sock_file, char* buf, int buf_len)
{
    if (NULL == buf || buf_len <= 0) {
        LOG(ERROR) << "AgentServer onRecv, get err data, buf is null or buf_len = 0, buf_len = " << buf_len;
        return;
    }

    //防止内存越界
    if (size_t(buf_len) < sizeof(LogData)) {
        LOG(WARNING) << "get data, err len " << buf_len;
        return;
    }

    //解析LogData
    LogData* data = reinterpret_cast<LogData*>(buf);

    //数据量不大，直接处理转发，后期可太难家一个缓存队列
    //...

    //common key
    log_common_data_.info_BIZ_type = conf.get_log_sdk_type();
    log_common_data_.server_type = data->server_type;
    log_common_data_.group_id = data->group_id;
    log_common_data_.server_id = data->server_id;
    log_common_data_.user_id = data->user_id;
    int ip_len = (strlen(data->ip) > IP_LEN) ? IP_LEN : strlen(data->ip);
    log_common_data_.server_ip = std::string(data->ip, ip_len);
    //extra key
    log_data_->set_common_field(&log_common_data_);
    size_t log_len = (strlen(data->log_data) > LOG_DATA_LEN) ? LOG_DATA_LEN : strlen(data->log_data);
    log_data_->clear_extra_common_field();
    log_data_->add_extra_common_field("log_data", data->log_data, log_len);
    LOG(INFO) << "send data " << log_data_->get_json_log();
    RET_NUM ret = log_sdk_->log(log_data_);
    if (RET_NUM_SUCCESS != ret) {
        LOG(ERROR) << "log_sdk log err, " << ret;
        return;
    }

    return;
}

void AgentServer::onError(int error_code)
{
    LOG(ERROR) << "AgentServer onError, err code = " << error_code;
}

void AgentServer::close()
{
    usock_server_->close();
    delete usock_server_;
}
