#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>
#include <string>

#include "iniparser.h"

class Config
{
public:
    Config();
    ~Config();

    void load(const char* path);

    std::string get_local_ip() {
        return local_ip_;
    }
    std::string get_process_name() {
        return process_name_;
    }
    std::string get_uds_file() {
        return uds_file_;
    }
    std::string get_log_path() {
        return log_path_;
    }
    int32_t get_log_level() {
        return log_level_;
    }
    int32_t get_log_max_size() {
        return log_max_size_;
    }
    std::string get_log_sdk_host() {
        return log_sdk_host_;
    }
    std::string get_log_sdk_name() {
        return log_sdk_name_;
    }
    std::string get_log_biz_type() {
        return log_biz_type_;
    }
    std::string get_log_sdk_version() {
        return log_sdk_version_;
    }

private:
    dictionary* ini_;

    std::string local_ip_;          //本机ip
    std::string process_name_;      //进程名，一台机器部署多个需要区分名称

    std::string uds_file_;          //uds文件名

    std::string log_path_;          //日志路径+日志名
    int32_t log_level_;             //日志等级
    int32_t log_max_size_;          //日志文件大小

    //logsdk
    std::string log_kafka_http_path_;  // 本地向某个 log kafka 获取所有的 log kafka 集合
    std::string log_kafka_name_;       // log kafka 向 etcd 注册时所使用的service name

    std::string log_biz_type_;         // logsdk传给哪个topic
    std::string log_sdk_version_;      // v0.1

};

#endif
