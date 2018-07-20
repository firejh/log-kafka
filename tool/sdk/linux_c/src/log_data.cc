
#include <stdint.h>

#include <cstdio>
#include <cstring>

#include <string>

//opensource
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
//common
#include "time/time.h"
#include "lock/rw_lock.h"

#include "log_data.h"

namespace log_collect{

//数值都按64处理
static std::string to_string(uint64_t value)
{
    char temp[32] = {0};
    sprintf(temp, "%llu", value);
    return temp;
}

bool create_LogData(ILogData** p_obj)
{
    LogData* tmp = new LogData();
    *p_obj = tmp;

    return true;
}

LogData::LogData()
{
    using namespace common;
    RW_lock_ = new RWLock();
}

bool LogData::set_common_field(const CommonLogData* common_log_data)
{
    using namespace common;
    RWLockScopeGuard lock(*RW_lock_, RWLOCK_WRITE);
    common_data_ = *common_log_data;

    return true;
}

void LogData::clear_extra_common_field()
{
    using namespace common;
    RWLockScopeGuard lock(*RW_lock_, RWLOCK_WRITE);
    extra_common_data_.clear();
}

bool LogData::add_extra_common_field(const char* key, const char* value)
{
    using namespace common;
    RWLockScopeGuard lock(*RW_lock_, RWLOCK_WRITE);
    if (NULL == key || NULL == value) {
        return false;
    }
    extra_common_data_[std::string(key, strlen(key))] = std::string(value, strlen(value));

    return true;
}

bool LogData::add_extra_common_field(const char* key, const uint64_t value)
{
    char temp[32] = {0};
    sprintf(temp, "%llu", (long long unsigned int)value);
    return add_extra_common_field(key, temp);
}

std::string LogData::get_json_log()
{
    using namespace rapidjson;
    using namespace common;

    if (common_data_.info_BIZ_type.empty()) {
        return "";
    }

    RWLockScopeGuard lock(*RW_lock_, RWLOCK_READ);
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();

    writer.Key("BizType");
    writer.String(common_data_.info_BIZ_type.c_str());

    writer.Key("time");
    writer.String(to_string(time(NULL)).c_str());

    if (0 != common_data_.info_type) {
        writer.Key("infoType");
        writer.String(to_string(common_data_.info_type).c_str());
    }

    if (!common_data_.info_desc.empty()) {
        writer.Key("desc");
        writer.String(common_data_.info_desc.c_str());
    }

    if (0 != common_data_.group_id) {
        writer.Key("groupId");
        writer.String(to_string(common_data_.group_id).c_str());
    }

    if (0 != common_data_.server_id) {
        writer.Key("serviceId");
        writer.Uint(common_data_.server_id);
    }

    if (!common_data_.server_name.empty()) {
        writer.Key("serviceName");
        writer.String(common_data_.server_name.c_str());
    }

    if (!common_data_.server_ip.empty()) {
        writer.Key("serviceIp");
        writer.String(common_data_.server_ip.c_str());
    }

    if (0 != common_data_.user_id) {
        writer.Key("userId");
        writer.String(to_string(common_data_.user_id).c_str());
    }

    if (0 != common_data_.room_id) {
        writer.Key("roomId");
        writer.String(to_string(common_data_.room_id).c_str());
    }

    if (!common_data_.session_id.empty()) {
        writer.Key("sessionId");
        writer.String(common_data_.session_id.c_str());
    }

    writer.EndObject();

    return s.GetString();
}

}
