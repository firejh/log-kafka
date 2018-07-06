#ifndef LOG_COLLET_I_SDK_H
#define LOG_COLLET_I_SDK_H

#include <string>

#include "i_log_data.h"

namespace log_collect{

class ILogCollectSDK;
bool create_LogCollectSDK(ILogCollectSDK** p_obj);

enum RET_NUM
{
    RET_NUM_SUCCESS = 0,
    RET_NUM_UDP_ERR = -1,
    RET_NUM_SEND_UDP_ERR = -2,
    RET_NUM_GET_JSON_ERR = -3,
    RET_GET_LOG_SERVER_ERR = -4,
};

using namespace std;

class ILogCollectSDK
{
public:
    //open
    //local_ip: 本机ip
    //log_server_host: log-server列表获取地址
    //service_name，必须使用自己的，需要申请
    //service_version, v0.1
    virtual RET_NUM open(string local_ip, string log_server_host, string service_name, string service_version) = 0;

    //发送日志
    virtual RET_NUM log(ILogData* log_data) = 0;

    //设置异步缓存队列size，默认1024 * 32，可以不设置
    virtual void set_cache_size(size_t size) = 0;

    //设置，异步获取logserver更新信息的时间间隔，默认60s，可以不设置
    virtual void set_check_timeval(uint32_t sec) = 0;

    virtual ~ILogCollectSDK(){}
};//end ILogCollectSDK

}//end log_collect
#endif
