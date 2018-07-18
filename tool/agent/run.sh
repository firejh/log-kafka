ulimit -c unlimited
export LD_LIBRARY_PATH=./import/lib
./bin/log_kafka_agent conf/log_kafka_agent.ini
