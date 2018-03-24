#!/usr/bin/env bash
# ******************************************************
# DESC    :
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-03-24 17:22
# FILE    : stop.sh
# ******************************************************

dir=`pwd`
nginx -p $dir -c conf/log_kafka.conf -s stop
