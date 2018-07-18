#ifndef LOG_COLLET_I_DATA_H
#define LOG_COLLET_I_DATA_H

#include "stdint.h"
#include <string>

namespace log_collect{

class ILogData;
bool create_LogData(ILogData** p_obj);

struct CommonLogData
{
    std::string info_BIZ_type;  //使用的topic，必填
    uint32_t info_type;         //信息类型
    std::string info_desc;      //描述

    uint32_t server_type;       //服务类型
    uint32_t group_id;          //组id
    uint32_t server_id;         //服务id
    std::string server_name;    //服务名称
    std::string server_ip;      //服务ip

    uint64_t user_id;           //用户id
    uint64_t room_id;           //房间id
    std::string session_id;     //用户自填，扩展字段

    CommonLogData()
    {
        info_type = 0;
        server_type = 0;
        group_id = 0;
        server_id = 0;
        user_id = 0;
        room_id = 0;
    }
};

class ILogData {
public:
    //设置公共字段，必须设置，每次设置会覆盖老的
    virtual bool set_common_field(const CommonLogData* common_log_data) = 0;

    //清空私有字段，如果私有字段变化需要先清空，后重新add_extra_common_field
    virtual void clear_extra_common_field() = 0;

    //添加私有字段
    virtual bool add_extra_common_field(const char* key, const char* value, size_t value_len) = 0;
    virtual bool add_extra_common_field(const char* key, const uint64_t value) = 0;

    //获取log的json格式字符串
    virtual std::string get_json_log() = 0;

    virtual ~ILogData(){}
};

}

#endif
