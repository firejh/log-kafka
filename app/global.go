/******************************************************
# DESC    : global variables
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-03-22 20:43
# FILE    : global.go
******************************************************/

package main

import (
	"sync/atomic"
	"time"
)

import (
	"github.com/AlexStocks/goext/database/filter"
	"github.com/AlexStocks/goext/database/registry"
	"github.com/AlexStocks/goext/log"
)

const (
	logBizType     = "pplog-biztype"
	logBizVersion  = "pplog-bizversion"
	logBizProtocol = "pplog-protocol"
	logZipType     = "pplog-zip"
	logText        = "pplog-log"
)

type (
	empty interface{}
)

var (
	// local ip
	LocalIP string
	// progress id
	ProcessID string
	// Conf is main config
	Conf ConfYaml
	// Log records server request log
	Log gxlog.Logger
	// Log records kafka log
	KafkaLog gxlog.Logger
	// Log records http requests
	HTTPLog gxlog.Logger
	// StatStorage implements the storage interface
	StatStorage *Storage
	// now in unix time
	Now int64
	// kafka message pusher worker
	Worker *KafkaWorker
	// udp server
	Server *UdpServer
	// register
	Register gxregistry.Registry
	// filter
	Filter gxfilter.Filter
	//kafka info keeper
	kafkaInfoKeeper *KafkaInfoKeeper
	//monitor
	Monitor *LogMonitor
)

func UpdateNow() {
	atomic.StoreInt64(&Now, int64(time.Now().Unix()))
}
