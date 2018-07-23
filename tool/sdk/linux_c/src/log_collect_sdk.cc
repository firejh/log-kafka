//common
// #include "udp/udp_socket.h"

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
//#include <netinet/in.h>
//#include <fcntl.h>

#include <string>
#include <iostream>

#include "log_collect_sdk.h"
#include "log_server.h"

namespace log_collect{

enum RET_UDP_NUM
{
    RET_UDP_NUM_SUCCESS = 0,
    RET_UDP_NUM_SOCK = 1,               //创建socket失败
    RET_UDP_NUM_SET_BUF = 2,            //设置缓冲
    RET_UDP_NUM_SEND = 3,               //发送失败


    RET_UDP_NUM_USER_DATA_NULL = 10,    //用户数据为null
};

#ifdef _linux
#define UDP_SOCK_SEND_BUF (1024 * 1024 * 32)
#else
#define UDP_SOCK_SEND_BUF (1024 * 1024 * 4)
#endif

std::string GetIPString(uint32_t net_n_ip)
{
    struct  in_addr addr;
    memset(&addr,0,sizeof(struct in_addr));
    addr.s_addr = net_n_ip;
    return inet_ntoa(addr);
}

//创建send udp socket
int create_send_udp_socket()
{
    int send_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (send_sock < 0) {
        return -1;
    }

    int liRecvBuf = UDP_SOCK_SEND_BUF;
    int liResult = setsockopt(send_sock, SOL_SOCKET, SO_SNDBUF, (const char*)&liRecvBuf, sizeof(int));
    if (liResult != 0) {
        return -1;
    }

    return send_sock;
}


RET_UDP_NUM send_udp_data(int local_sock, uint32_t to_ip, uint16_t to_port, const void* data, uint16_t data_len)
{
    if (NULL == data || 0 == data_len) {
        return RET_UDP_NUM_USER_DATA_NULL;
    }

    struct sockaddr_in sin = {0};
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = to_ip;
    sin.sin_port = to_port;

    ssize_t size = 0;
    char* p_data = (char*)(void*)(data);
    while (true) {
        size = ::sendto(local_sock, (char*)p_data, data_len, 0, (struct sockaddr*)&sin, sizeof(sin));
        if (ssize_t(-1) == size) {
            return RET_UDP_NUM_SEND;
        } else if (size == data_len) {
            break;
        }

        data_len -= size;
        p_data = p_data + (size_t)(size);
    }

    return RET_UDP_NUM_SUCCESS;
}

bool create_LogCollectSDK(ILogCollectSDK** p_obj)
{
    LogCollectSDK* temp = new LogCollectSDK();
    *p_obj = temp;

    return true;
}

LogCollectSDK::LogCollectSDK()
{
    running_ = false;
    send_sock_ = -1;
    check_timeval_ = 60;
    pthread_mutex_init(&global_lock_, NULL);
    queue_max_size_ = 1024 * 32;
}

RET_NUM LogCollectSDK::open(string local_ip, string log_server_host, string service_name, string service_version)
{
    running_ = true;
    local_ip_ = local_ip;
    log_server_host_ = log_server_host;
    service_name_ = service_name;
    service_version_ = service_version;
    //启动的时候不拉取log server，如果logserver异常必须报关证使用者正常使用，这里认为log可以丢失，但是不能影响使用者
    pthread_create(&monitor_thread_, NULL, &th_monitor_log_server, (void*)this);
    pthread_create(&send_thread_, NULL, &th_send_log, (void*)this);

    send_sock_ = create_send_udp_socket();
    if (send_sock_ < 0) {
        return RET_NUM_UDP_ERR;
    }

    return RET_NUM_SUCCESS;
}

void* LogCollectSDK::th_monitor_log_server(void* data)
{
    ((LogCollectSDK*)data)->monitor_log_server();
    return NULL;
}

void LogCollectSDK::monitor_log_server(void)
{
    std::vector<Node> servers;
    while (running_) {
        servers.clear();
        LogNodesErrorCode ret = (LogNodesErrorCode)get_log_nodes(local_ip_, log_server_host_, service_name_, service_version_, servers);
        if (GetLogNodesSuccess != ret || servers.empty()) {
            std::cout << "err, monitor_log_server empty, ret = " << ret << "\n";
            sleep(check_timeval_);
            continue;
        }

        //先比较后修改,比较不需要加锁，因为只有本线程会写入
        bool flag = false;
        if (to_address_.size() != servers.size()) {
            flag = true;
        } else {
            for (size_t i = 0; i < servers.size(); ++i) {
                Node& node = servers[i];
                //这里就做嵌套遍历即可，性能不需要关心
                bool find = false;
                for (int j = 0; j < to_address_.size(); ++j) {
                    std::pair<int, uint16_t>& local_server = to_address_[j];
                    if (inet_addr(node.address().c_str()) == local_server.first && ntohs(node.port()) == local_server.second) {
                        find = true;
                        break;
                    }
                }

                if (false == find) {
                    flag = true;
                    break;
                }
            }
        }

        if (false == flag) {
            sleep(check_timeval_);
            continue;
        }

        do {
            //这里只有变化的时候会执行，可以忽略加锁的性能影响
            pthread_mutex_lock(&global_lock_);
            to_address_.clear();
            for (size_t i = 0; i < servers.size(); ++i) {
                add_log_server(servers[i].address().c_str(), uint16_t(servers[i].port()));
            }
            pthread_mutex_unlock(&global_lock_);
        } while(0);

        sleep(check_timeval_);
    }

}

void LogCollectSDK::add_log_server(const char* to_ip, uint16_t to_port)
{
    if (0 == to_port || NULL == to_ip) {
        return;
    }

    to_address_.push_back(std::make_pair(inet_addr(to_ip), htons(to_port)));
}

RET_NUM LogCollectSDK::log(ILogData* log_data)
{
    pthread_mutex_lock(&queue_lock_);
    if (queue_.size() < queue_max_size_) {
        std::string* data = new std::string();
        *data = log_data->get_json_log();
        queue_.push_back(data);
    }
    pthread_mutex_unlock(&queue_lock_);

    return RET_NUM_SUCCESS;
}

void* LogCollectSDK::th_send_log(void* data)
{
    ((LogCollectSDK*)data)->send_log();
    return NULL;
}

void LogCollectSDK::send_log(void)
{
    std::cout << "LogCollectSDK send thread start...\n";
    static uint32_t send_seq = 0;
    while (running_) {
        std::vector<string*> temp;
        //lock
        pthread_mutex_lock(&queue_lock_);
        queue_.swap(temp);
        queue_.reserve(queue_max_size_);
        pthread_mutex_unlock(&queue_lock_);
        //lock end

        if (temp.empty()) {
            usleep(100000);
            continue;
        }

        for (size_t i = temp.size(); i > 0; --i) {
            std::string* data = temp[i - 1];
            if (data == NULL) {
                continue;
            }
            //lock
            pthread_mutex_lock(&global_lock_);
            if (!to_address_.empty()) {
                std::pair<int, uint16_t> log_server = to_address_[send_seq++ % to_address_.size()];
                try {
                    if (send_udp_data(send_sock_, log_server.first, log_server.second, data->c_str(), data->size()) != RET_UDP_NUM_SUCCESS) {
                        //发送失败就扔了，需要的话可以加个用户回调
                        std::cout << "log_collet err, send data failed\n";
                    } else {
                        //std::cout << "send data to " << GetIPString(log_server.first) << ":" << ntohs(log_server.second) << std::endl;
                    }
                }catch (std::exception& ex) {
                    std::cout << "log_collet excepton : " << ex.what() << "\n";
                }
                delete data;
            }
            pthread_mutex_unlock(&global_lock_);
            //lock end
        }
    }
    std::cout << "LogCollectSDK send thread stop \n";

    return;
}

void LogCollectSDK::set_cache_size(size_t size) {
    if (size < 1024) {
        return;
    }

    queue_max_size_ = size;
}

void LogCollectSDK::set_check_timeval(uint32_t sec)
{
    if (sec < 10) {
        return;
    }
    check_timeval_ = sec;
}

void LogCollectSDK::close(void*)
{
    running_ = false;
    pthread_join(monitor_thread_, NULL);
    pthread_join(send_thread_, NULL);
}

}

