/*
 * description: 通过uds接收日志，然后待用log-kafka的sdk打入kafka
 *
*/

#ifndef __AGENT_SERVER_H__
#define __AGENT_SERVER_H__

#include <stdint.h>
#include <iostream>
#include "uds_datagram.h"
#include "i_log_collect_sdk.h"
#include "global.h"

//宏定义
#define IP_LEN 32
#define LOG_DATA_LEN 4096

#pragma pack(1)

struct AgentServerEXception : public std::exception {
    AgentServerEXception(std::string data) {
        data_ = data;
    }
    virtual ~AgentServerEXception() throw() {}
    const char* what() const throw() {
        return data_.c_str();
    }
    std::string data_;
};

//uds 包协议
struct LogData {
    uint32_t server_type;       //服务类型
    uint32_t group_id;          //组id
    uint32_t server_id;         //server id
    uint64_t user_id;           //用户id

    char ip[IP_LEN];            //ip

    char log_data[LOG_DATA_LEN];
};
#pragma pack()

using namespace log_collect;

class AgentServer : public UDSDatagramCallback {
public:
    AgentServer();
    ~AgentServer();

    void open();
    void close();

    virtual void onRecv(const char * peer_sock_file, char* buf, int buf_len);
    virtual void onError(int error_code);

private:
    UDSDatagramServer* usock_server_;               //uds server
    ILogCollectSDK* log_sdk_;                       //日志sdk
    CommonLogData log_common_data_;                 //日志数据的公共数据（sdk必填）
    ILogData* log_data_;                            //日志公共数据+私有数据
};

#endif
