#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

#include "log_server.h"
#include "i_log_collect_sdk.h"

int main(int argc, char* argv[])
{
    using namespace log_collect;
    std::string local_ip = "127.0.0.1";
    std::string server_log_host = "127.0.0.1:3000";
    std::string service_name = "test";
    std::string version = "v0.1";
    if (argc == 3) {
        local_ip = argv[1];
        server_log_host = argv[2];
    }

    //准备sdk
    ILogCollectSDK* log_sdk = NULL;
    if (create_LogCollectSDK(&log_sdk)) {
        std::cout << "create_LogCollectSDK success\n";
    } else {
        std::cout << "create_LogCollectSDK fail\n";
    }
    log_sdk->open(local_ip, server_log_host, service_name, version);

    //准备需要的日志公共字段
    CommonLogData common_data;
    common_data.info_BIZ_type = "telemetry"; //必须填，使用规定好的topic
    common_data.info_type = 0;                      //信息类型
    common_data.info_desc = "desc";                 //描述
    common_data.server_type = 0;                    //服务类型
    common_data.group_id = 0;
    common_data.server_name = "test";
    common_data.server_ip = "localhost";
    common_data.user_id = 0;
    common_data.room_id = 0;
    common_data.session_id = "sessionID";

    std::cout << "info_BIZ_type = " << common_data.info_BIZ_type << std::endl;
    //生成自己的日志数据对象，该对象线程安全，可以多线程使用一个对象，使用读写所
    ILogData* log_data = NULL;
    if (create_LogData(&log_data)) {
        std::cout << "create_LogData success\n";
    } else {
        std::cout << "create_LogData fail\n";
        return -1;
    }

    log_data->set_common_field(&common_data);

    //添加自己的特需日志，注意字段名使用骆驼命名法，统一规范
    log_data->add_extra_common_field("testLog1", "this is string");
    log_data->add_extra_common_field("testLog2", 100000000);

    std::cout << "my log formart: " << log_data->get_json_log() << std::endl;

    return;
    while (1) {
        //如果需要去掉老字段需要clear_extra_common_field，如果不需要则不要clear_extra_common_field，新的会覆盖老字段的value
        log_data->clear_extra_common_field();
        log_data->add_extra_common_field("testLog1", "this is string");
        log_data->add_extra_common_field("testLog2", 100000000);
        //发送日志
        RET_NUM ret = log_sdk->log(log_data);
        if (RET_NUM_SUCCESS == ret) {
            std::cout << "send log: " << log_data->get_json_log() << " success" << std::endl;
        } else {
            std::cout << "send log: " << log_data->get_json_log() << " fail, ret:" << ret << std::endl;
        }
        sleep(1);
    }
    return 1;
}