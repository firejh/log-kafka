package main

import (
	"time"
)

import (
	"github.com/AlexStocks/goext/log"
)

type (
	empty interface{}
)

var (
	// local ip
	LocalIP   string
	LocalHost string
	// progress id
	ProcessID string
	// Conf is main config
	Conf ConfYaml
	// Log records server request log
	Log gxlog.Logger
	// now
	Now time.Time
	// kafka message pusher worker
	Worker *KafkaWorker
	// udp server
	Server *UdpServer
)

func UpdateNow() {
	Now = time.Now()
}
