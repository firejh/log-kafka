#ifndef COLLECT_LOG_DATA_H
#define COLLECT_LOG_DATA_H

#include "i_log_data.h"

#include <map>

//前置声明
namespace common {
    class RWLock;
}

namespace log_collect{

class LogData : public ILogData
{
public:
    //设置公共字段，必须设置，每次设置会覆盖老的
    virtual bool set_common_field(const CommonLogData* common_log_data);

    //清空私有字段，如果私有字段变化需要先清空，后重新add_extra_common_field
    virtual void clear_extra_common_field();

    //添加私有字段
    virtual bool add_extra_common_field(const char* key, const char* value);
    virtual bool add_extra_common_field(const char* key, const uint64_t value);

    //获取log的json格式字符串
    virtual std::string get_json_log();

public:
    LogData();
    //~LogData(){}

private:
    CommonLogData common_data_;
    std::map<std::string, std::string> extra_common_data_;
    common::RWLock* RW_lock_;
};

}

#endif
