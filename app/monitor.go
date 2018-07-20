package main

import (
	"fmt"
	"sync"
	"time"
)

import (
	"github.com/divebomb/monitor/api/go"
)

type LogMonitor struct {
	done chan empty
	wg   sync.WaitGroup
}

func NewLogMonitor() *LogMonitor {
	m := &LogMonitor{
		done: make(chan empty),
	}
	return m
}

func (c *LogMonitor) Stop() {
	close(c.done)
	c.wg.Wait()
}

func (c *LogMonitor) Start() {
	c.wg.Add(1)
	defer c.wg.Done()
	var (
		err error
		//total qps
		total_qps_start = StatStorage.GetTotalCount()
		total_qps_end   = StatStorage.GetTotalCount()
		//udsp sucessqps
		udp_qps_success_start = StatStorage.GetUdpSuccess()
		udp_qps_success_end   = StatStorage.GetUdpSuccess()
		udp_qps_err_start     = StatStorage.GetUdpError()
		udp_qps_err_end       = StatStorage.GetUdpError()
		//http qps
		http_qps_success_start = StatStorage.GetHttpSuccess()
		http_qps_success_end   = StatStorage.GetHttpSuccess()
		http_qps_err_start     = StatStorage.GetHttpError()
		http_qps_err_end       = StatStorage.GetHttpError()
	)

	//create monitor
	monitor := monitor_api.NewMonitor()
	err = monitor.Start()
	if err != nil {
		panic(fmt.Sprintf("monitor start err %v", err))
	}

	//ticker
	t := time.NewTicker(time.Second * 60)
LOOP:
	for {
		select {
		case <-c.done:
			Log.Warn("Log monitor stop now")
			break LOOP

		case <-t.C:
			total_qps_end = StatStorage.GetTotalCount()
			udp_qps_success_end = StatStorage.GetUdpSuccess()
			udp_qps_err_end = StatStorage.GetUdpError()
			http_qps_success_end = StatStorage.GetHttpSuccess()
			http_qps_err_end = StatStorage.GetHttpError()
			//监控数据推送
			monitor.Monitor(monitor_api.MonitorData{
				OpType:      monitor_api.MONITOR_OP_TYPE_LAST,
				ServiceName: "LogCollectServer",
				AttrName:    "TotalQPS",
				ServerID:    int64(Conf.Core.ServerIntID),
				Value:       (total_qps_end - total_qps_start),
			})
			monitor.Monitor(monitor_api.MonitorData{
				OpType:      monitor_api.MONITOR_OP_TYPE_LAST,
				ServiceName: "LogCollectServer",
				AttrName:    "UDPSuccessQPS",
				ServerID:    int64(Conf.Core.ServerIntID),
				Value:       (udp_qps_success_end - udp_qps_success_start),
			})
			monitor.Monitor(monitor_api.MonitorData{
				OpType:      monitor_api.MONITOR_OP_TYPE_LAST,
				ServiceName: "LogCollectServer",
				AttrName:    "UDPErrQPS",
				ServerID:    int64(Conf.Core.ServerIntID),
				Value:       (udp_qps_err_end - udp_qps_err_start),
			})
			monitor.Monitor(monitor_api.MonitorData{
				OpType:      monitor_api.MONITOR_OP_TYPE_LAST,
				ServiceName: "LogCollectServer",
				AttrName:    "HttpSuccessQPS",
				ServerID:    int64(Conf.Core.ServerIntID),
				Value:       (http_qps_success_end - http_qps_success_start),
			})
			monitor.Monitor(monitor_api.MonitorData{
				OpType:      monitor_api.MONITOR_OP_TYPE_LAST,
				ServiceName: "LogCollectServer",
				AttrName:    "HttpErrQPS",
				ServerID:    int64(Conf.Core.ServerIntID),
				Value:       (http_qps_err_end - http_qps_err_start),
			})

			total_qps_start = StatStorage.GetTotalCount()
			udp_qps_success_start = StatStorage.GetUdpSuccess()
			udp_qps_err_start = StatStorage.GetUdpError()
			http_qps_success_start = StatStorage.GetHttpSuccess()
			http_qps_err_start = StatStorage.GetHttpError()
		}
	}
}
