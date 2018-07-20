#include <exception>

#include "config.h"

Config::Config()
{
    ini_ = NULL;
}

Config::~Config()
{
    if (NULL != ini_) {
        iniparser_freedict(ini_);
    }
}

void Config::load(const char* path)
{
    ini_ = iniparser_load(path);
    if (NULL == ini_) {
        throw "iniparser_load, NULL";
    }

    local_ip_ = iniparser_getstring(ini_, "server:local_ip", "127.0.0.1");
    process_name_ = iniparser_getstring(ini_, "server:process_name", "log_kafka_agent");

    uds_file_ = iniparser_getstring(ini_, "uds:sock_name", "/data/server/log_kafka_agent/log_kafka_agent.sock");

    log_path_ = iniparser_getstring(ini_, "glog:log_path", "/data/logs/log_kafka_agent/log_kafka_agent");
    log_level_ = iniparser_getint(ini_, "glog:log_level", 0);
    log_max_size_ =  iniparser_getint(ini_, "glog:log_file_size", 1000);//M

    log_kafka_host_ = iniparser_getstring(ini_, "logsdk:host", "127.0.0.1:3000");
    log_service_name_ = iniparser_getstring(ini_, "logsdk:name", "cluster1");
    log_biz_type_ = iniparser_getstring(ini_, "logsdk:type", "log_kafka_agent_topic");
    log_service_version_ = iniparser_getstring(ini_, "logsdk:version", "v0.1");

    //iniparser_freedict(ini_)
}
