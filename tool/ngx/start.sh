#!/usr/bin/env bash
# ******************************************************
# DESC    :
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2017-02-22 20:33
# FILE    : start.sh
# ******************************************************

mkdir -p ./logs
dir=`pwd`
nohup nginx -p $dir -c conf/log_kafka.conf 1>logs/ngx.nohup 2>&1 &
