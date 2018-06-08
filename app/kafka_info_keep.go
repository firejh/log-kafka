package main

import (
	"context"
	"time"
	"sync"
)

import (
	"github.com/coreos/etcd/client"
	"strings"
)

type (
	EtcdKafaInfo struct {
		Key					string
		Brokers				string
		BrokersRWLock		sync.RWMutex
		UdpTopics			string
		UdpTopicMap			map[string] bool
		UdpTopicsRWLock 	sync.RWMutex
		HttpTopics			string
		HttpTopicMap		map[string] bool
		HttpTopicsRWLock 	sync.RWMutex

		workers			map[int64] bool	//运行中map的key初始化后不会变动，只有对应的value可能变化，不用加锁不需要原子操作
	}

	KafkaInfoKeep struct {
		cli		client.Client
		st		bool
	}

)

var (
	//etcd kafka info
	KafkaInfo EtcdKafaInfo
	dialTimeout    = 5 * time.Second
	requestTimeout = 10 * time.Second
	loopSleepTime = 60 * time.Second
)

func NewKafkaInfoKeeper() (*KafkaInfoKeep, error) {
	var (
		server *KafkaInfoKeep
		err error
	)

	cfg := client.Config{
		Endpoints:               Conf.Etcd.Addrs,
		Transport:               client.DefaultTransport,
		// set timeout per request to fail fast when the target endpoint is unavailable
		HeaderTimeoutPerRequest: dialTimeout,
	}
	server = &KafkaInfoKeep{st: true}
	server.cli, err = client.New(cfg)
	if err != nil {
		return nil, err
	}

	return server, nil
}

func (c *KafkaInfoKeep) Start() error{
	kapi := client.NewKeysAPI(c.cli)
	resp, err := kapi.Get(context.Background(), Conf.Etcd.KafkaInfoKey,nil)
	Log.Info("get kafka info from etcd, info %s:%s", resp.Node.Key, resp.Node.Value)
	if  err != nil {
		 return err
	}
	KafkaInfo.Brokers = resp.Node.Value

	//udp topic, http topic init
	resp, err = kapi.Get(context.Background(), Conf.Etcd.UdpTopicKey, nil)
	if err != nil {
		return err
	}
	KafkaInfo.UdpTopics = resp.Node.Value
	KafkaInfo.UdpTopicMap = make(map[string]bool)
	UTopics := strings.Split(resp.Node.Value, ",")
	for _, v := range UTopics {
		KafkaInfo.UdpTopicMap[v] = true
	}

	resp, err = kapi.Get(context.Background(), Conf.Etcd.HttpTopicKey, nil)
	if err != nil {
		return err
	}
	KafkaInfo.HttpTopics = resp.Node.Value
	KafkaInfo.HttpTopicMap = make(map[string]bool)
	HTopics := strings.Split(resp.Node.Value, ",")
	for _, v := range HTopics {
		KafkaInfo.HttpTopicMap[v] = true
	}

	//watch kafka info
	go func() {
		for {
			time.Sleep(loopSleepTime)
			resp, err := kapi.Get(context.Background(), Conf.Etcd.KafkaInfoKey,nil)
			if err != nil {
				Log.Error("get kafka info from etcd failed, error = %v", err)
				continue
			}
			Log.Info("get kafka info from etcd, info %s:%s", resp.Node.Key, resp.Node.Value)
			if KafkaInfo.Brokers == resp.Node.Value {
				continue
			} else {
				Log.Warn("etcd kafka info changed from %s to %s", KafkaInfo.Brokers, resp.Node.Value)
				KafkaInfo.BrokersRWLock.Lock()

				KafkaInfo.Brokers = resp.Node.Value
				for k,_ := range KafkaInfo.workers {
					KafkaInfo.workers[k] = false
				}
				KafkaInfo.BrokersRWLock.Unlock()
				//fmt.Printf("etcd kafka info changed from %s to %s\n", KafkaInfo.Brokers, resp.Node.Value)
			}
		}
	}()

	//watch udp topics
	go func() {
		for {
			time.Sleep(loopSleepTime)
			resp, err := kapi.Get(context.Background(), Conf.Etcd.UdpTopicKey, nil)
			if err != nil {
				Log.Error("get udp topic from etcd failed, error = %v", err)
				continue
			}
			Log.Info("get udp topic %s", resp.Node.Value)
			if (KafkaInfo.UdpTopics == resp.Node.Value) {
				continue
			} else {
				Log.Warn("etcd udptopics changed from %s to %s", KafkaInfo.UdpTopics, resp.Node.Value)
				KafkaInfo.UdpTopicsRWLock.Lock()
				KafkaInfo.UdpTopics = resp.Node.Value
				//拆分到map
				KafkaInfo.UdpTopicMap = make(map[string]bool)
				UTopics := strings.Split(resp.Node.Value, ",")
				for _, v := range UTopics {
					KafkaInfo.UdpTopicMap[v] = true
				}
				KafkaInfo.UdpTopicsRWLock.Unlock()
			}
		}
	}()

	//watch http topics
	go func() {
		for {
			time.Sleep(loopSleepTime)
			resp, err := kapi.Get(context.Background(), Conf.Etcd.HttpTopicKey, nil)
			if err != nil {
				Log.Error("get http topic from etcd failed, error = %v", err)
				continue
			}
			Log.Info("get http topic %s", resp.Node.Value)
			if (KafkaInfo.HttpTopics == resp.Node.Value) {
				continue
			} else {
				Log.Warn("etcd udptopics changed from %s to %s", KafkaInfo.HttpTopics, resp.Node.Value)
				KafkaInfo.HttpTopicsRWLock.Lock()
				KafkaInfo.HttpTopics = resp.Node.Value
				HTopics := strings.Split(resp.Node.Value, ",")
				for _, v := range HTopics {
					KafkaInfo.HttpTopicMap[v] = true
				}
				KafkaInfo.HttpTopicsRWLock.Unlock()
			}
		}
	}()

	return nil
}

func (c *KafkaInfoKeep) Stop() {

}


