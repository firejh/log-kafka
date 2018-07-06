#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

#include "i_log_collect_sdk.h"

int main(int argc, char* argv[])
{
    using namespace log_collect;
    //准备需要的数据
    std::string to_ip1 = "127.0.0.1";
    std::string to_ip2 = "127.0.0.1";
    uint16_t to_port1 = 20000;
    uint16_t to_port2 = 20000;

    if (argc > 4) {
        to_ip1 = argv[1];
        to_port1 = atoi(argv[2]);
        to_ip2 = argv[3];
        to_port2 = atoi(argv[4]);
    }

    std::cout << "log server: " << to_ip1 << ":" << to_port1 << "," << to_ip2 << ":" << to_port2 << "\n";

    //准备sdk
    ILogCollectSDK* log_sdk = NULL;
    if (create_LogCollectSDK(&log_sdk)) {
        std::cout << "create_LogCollectSDK success\n";
    } else {
        std::cout << "create_LogCollectSDK fail\n";
    }
    log_sdk->open();
    log_sdk->add_log_server(to_ip1.c_str(), to_port1);
    log_sdk->add_log_server(to_ip2.c_str(), to_port2);

    //准备需要的日志公共字段
    CommonLogData common_data;
    common_data.info_BIZ_type = "test_kafka_topic"; //必须填，使用规定好的topic
    common_data.info_type = 0;                      //信息类型
    common_data.info_desc = "desc";                 //描述
    common_data.server_type = 0;                    //服务类型
    common_data.group_id = 0;
    common_data.server_name = "test";
    common_data.server_ip = "localhost";
    common_data.user_id = 0;
    common_data.room_id = 0;
    common_data.session_id = "sessionID";

    std::cout << "info_BIZ_type = " << common_data.info_BIZ_type.c_str();
    //生成自己的日志数据对象，该对象线程安全，可以多线程使用一个对象，使用读写所
    ILogData* log_data = NULL;
    if (create_LogData(&log_data)) {
        std::cout << "create_LogData success\n";
    } else {
        std::cout << "create_LogData fail\n";
        return -1;
    }

    log_data->set_common_field(&common_data);

    //添加自己的特需日志，注意字段名使用骆驼命名法，统一规范
    log_data->add_extra_common_field("testLog1", "this is string");
    log_data->add_extra_common_field("testLog2", 100000000);
    std::cout << "my log formart: " << log_data->get_json_log() << "\n";

    //测试规则
    uint32_t sleep_u = 1000000;
    if (argc > 5) {
        sleep_u = atoi(argv[5]);
    }
    uint32_t num = 10;
    if (argc > 6) {
        num = atoi(argv[6]);
    }
    uint32_t num_back = num;

    uint64_t start_time = time(NULL);
    while (num-- > 0) {
        //如果需要去掉老字段需要clear_extra_common_field，如果不需要则不要clear_extra_common_field，新的会覆盖老字段的value
        log_data->clear_extra_common_field();
        log_data->add_extra_common_field("testLog1", "this is string");
        log_data->add_extra_common_field("testLog2", 100000000);
        //发送日志
        RET_NUM ret = log_sdk->send_log(log_data);
        /*测试性能
        if (RET_NUM_SUCCESS == ret) {
            std::cout << "send log: " << log_data->get_json_log() << " success\n";
        } else {
            std::cout << "send log: " << log_data->get_json_log() << " fail\n";
        }
        usleep(sleep_u);
        */
    }
    uint64_t stop_time = time(NULL);

    uint32_t cost_time = stop_time - start_time;
    uint32_t qps = num_back / (cost_time>0 ? cost_time : 1);

    std::cout << "qps = " << qps << "/s\n";

    return 1;
}
