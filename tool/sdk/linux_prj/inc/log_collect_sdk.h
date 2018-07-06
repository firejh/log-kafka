#ifndef LOG_COLLET_SDK_H
#define LOG_COLLET_SDK_H

#include <vector>
#include <queue>
#include <string>

#include "i_log_collect_sdk.h"

namespace log_collect{

using namespace std;

class LogCollectSDK : public ILogCollectSDK
{
public:
    LogCollectSDK();
    ~LogCollectSDK(){};
    virtual RET_NUM open(string local_ip, string log_server_host, string service_name, string service_version);
    virtual RET_NUM log(ILogData* log_data);

    virtual void set_check_timeval(uint32_t sec);
    virtual void set_cache_size(size_t size);

private:
    static void* th_monitor_log_server(void* data);
    static void* th_send_log(void* data);
    void monitor_log_server(void);
    void send_log(void);
    virtual void add_log_server(const char* to_ip, uint16_t to_port);

private://data
    std::string local_ip_;
    std::string log_server_host_;
    string service_name_;
    string service_version_;
    uint32_t check_timeval_;

    //本地缓存
    std::vector<string*> queue_;
    size_t queue_max_size_;
    pthread_mutex_t queue_lock_;
    pthread_t send_thread_;

    pthread_mutex_t global_lock_;
    pthread_t monitor_thread_;
    int32_t send_sock_;
    std::vector<std::pair<int, uint16_t> > to_address_;
};//end LogCollectSDK

}//end log_collect
#endif
