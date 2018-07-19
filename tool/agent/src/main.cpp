#include <signal.h>
#include <sys/stat.h>

#include <glog/logging.h>

#include "iniparser.h"
#include "agent_server.h"
#include "global.h"

Config conf;

bool exit_flag = true;

void sig_handler(int sig)
{
    switch (sig) {
    case SIGINT:
        LOG(INFO) <<  "Received SIGINT scheduling shutdown...";
        break;
    case SIGTERM:
        LOG(INFO) << "Received SIGTERM scheduling shutdown...";
        break;
    default:
        LOG(INFO) << "Received shutdown signal, scheduling shutdown...";
    }
    exit_flag = false;

}

void set_signal_handlers(void)
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = sig_handler;
    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGINT, &act, NULL);
}

int main(int argc, char* argv[])
{
    try {
        //argv
        if (argc != 2 ) {
            printf("illegal argc, please input config file");
            return -1;
        }

        //signal
        signal(SIGHUP, SIG_IGN);
        signal(SIGPIPE, SIG_IGN);
        set_signal_handlers();

        //config
        std::string config_file = argv[1];
        dictionary* d = iniparser_load(config_file.c_str());
        if (NULL == d) {
            printf("open config %s failed", config_file.c_str());
            return -1;
        }
        conf.load(config_file.c_str());

        //log
        mkdir(conf.get_log_path().c_str(), 0755);
        google::InitGoogleLogging(conf.get_process_name().c_str());
        std::string log_file = conf.get_log_path() + "/INFO_";
        google::SetLogDestination(google::INFO, log_file.c_str());
        log_file = conf.get_log_path() + "/WARNING_";
        google::SetLogDestination(google::WARNING, log_file.c_str());
        log_file = conf.get_log_path() + "/ERROR_";
        google::SetLogDestination(google::FATAL, log_file.c_str());
        google::SetStderrLogging(google::GLOG_INFO);
        google::SetLogFilenameExtension("log_");
        FLAGS_max_log_size = conf.get_log_max_size();
        FLAGS_stop_logging_if_full_disk = true;
        FLAGS_minloglevel = conf.get_log_level();//INFO, WARNING，ERROR，FATAL

        //work start
        AgentServer s;
        s.open();

        //wait stop
        while (exit_flag) {
            sleep(1);
        }

        //work stop
        s.close();
    } catch (std::exception& e) {
        LOG(ERROR) << "get exception " << e.what();
        return -1;
    }

}
