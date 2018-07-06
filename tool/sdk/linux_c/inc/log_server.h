/******************************************************
# DESC    : get log server nodes
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-05-03 22:52
# FILE    : log_server.h
******************************************************/

#include <string>
#include <vector>

#include "service.pb.h"

using namespace std;


enum LogNodesErrorCode {
    GetLogNodesSuccess = 0,
    HTTPSendRequstFail = -1,
    IllegalHTTPRsp = -2,
    HTTPRspStatusError = -3,
    HTTPRspServiceError = -4,
    HTTPRspWrongAttr = -5,
    HTTPRspNodesEmpty = -6,
    PbjsonErrorBase = -7,
};

extern int get_log_nodes(string local_ip, string log_server_host,
            string service_name, string service_version, vector<Node>& nodes);