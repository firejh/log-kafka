package main

import (
	"context"
	"time"
	"sync"
)

import (
	"github.com/coreos/etcd/client"
	"strings"
	"fmt"
)

type (
	EtcdKafaInfo struct {
		Key					string
		Brokers				string
		BrokersRWLock		sync.RWMutex
		UdpTopics			string
		UdpTopicMap			map[string] bool
		UdpTopicsRWLock		sync.RWMutex
		HttpTopics			string
		HttpTopicMap		map[string] bool
		HttpTopicsRWLock	sync.RWMutex
		workers				map[int64] bool	//运行中map的key初始化后不会变动，只有对应的value可能变化，不用加锁不需要原子操作
	}

	KafkaInfoKeeper struct {
		cli		client.Client
		st		bool
		wg		sync.WaitGroup
		done	chan empty
	}

)

var (
	//etcd kafka info
	KafkaInfo EtcdKafaInfo
	dialTimeout    = 5 * time.Second
	requestTimeout = 5 * time.Second
	loopSleepTime = 5 * time.Second
)

func NewKafkaInfoKeeper() (*KafkaInfoKeeper, error) {
	var (
		server *KafkaInfoKeeper
		err error
	)

	cfg := client.Config{
		Endpoints:               Conf.Etcd.Addrs,
		Transport:               client.DefaultTransport,
		// set timeout per request to fail fast when the target endpoint is unavailable
		HeaderTimeoutPerRequest: dialTimeout,
	}
	server = &KafkaInfoKeeper{
		st: true,
		done: make(chan empty),}
	server.cli, err = client.New(cfg)
	if err != nil {
		return nil, err
	}

	return server, nil
}

func (c *KafkaInfoKeeper) Start() error{
	kapi := client.NewKeysAPI(c.cli)
	ctx, cancel := context.WithTimeout(context.Background(), requestTimeout)
	resp, err := kapi.Get(ctx, Conf.Etcd.KafkaInfoKey,nil)
	cancel()
	Log.Info("get kafka info from etcd, info %s:%s", resp.Node.Key, resp.Node.Value)
	if  err != nil {
		 return err
	}
	KafkaInfo.Brokers = resp.Node.Value

	//udp topic, http topic init
	ctx, cancel = context.WithTimeout(context.Background(), requestTimeout)
	resp, err = kapi.Get(ctx, Conf.Etcd.UDPTopicKey, nil)
	cancel()
	if err != nil {
		return err
	}
	KafkaInfo.UdpTopics = resp.Node.Value
	KafkaInfo.UdpTopicMap = make(map[string]bool)
	UTopics := strings.Split(resp.Node.Value, ",")
	for _, v := range UTopics {
		KafkaInfo.UdpTopicMap[v] = true
	}
	ctx, cancel = context.WithTimeout(context.Background(), requestTimeout)
	resp, err = kapi.Get(ctx, Conf.Etcd.HTTPTopicKey, nil)
	cancel()
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
	c.wg.Add(1)
	go c.watchBrokers()
	//watch udp topics
	c.wg.Add(1)
	go c.watchUDPTopics()
	//watch http topics
	c.wg.Add(1)
	go c.watchHTTPTopics()

	return nil
}

func (c *KafkaInfoKeeper) Stop() {
	close(c.done)
	c.wg.Wait()
}

func (c *KafkaInfoKeeper) watchBrokers() {
	kapi := client.NewKeysAPI(c.cli)
	t := time.NewTicker(loopSleepTime)
	for {
		select {
		case <-t.C:
			resp, err := kapi.Get(context.Background(), Conf.Etcd.KafkaInfoKey,nil)
			if err != nil {
				Log.Error("get kafka info from etcd failed, error = %v", err)
				continue
			}
			Log.Info("get kafka info from etcd, info %s:%s", resp.Node.Key, resp.Node.Value)
			if KafkaInfo.Brokers == resp.Node.Value {
				continue
			}

			Log.Warn("etcd kafka info changed from %s to %s", KafkaInfo.Brokers, resp.Node.Value)
			KafkaInfo.BrokersRWLock.Lock()
			KafkaInfo.Brokers = resp.Node.Value
			for k,_ := range KafkaInfo.workers {
				KafkaInfo.workers[k] = false
			}
			KafkaInfo.BrokersRWLock.Unlock()
			//fmt.Printf("etcd kafka info changed from %s to %s\n", KafkaInfo.Brokers, resp.Node.Value)

		case <-c.done:
			fmt.Println("stop#######")
			Log.Warn("watchBrokers stop")
			c.wg.Done()
			break
		}
	}
}

func (c *KafkaInfoKeeper) watchUDPTopics() {
	kapi := client.NewKeysAPI(c.cli)
	t := time.NewTicker(loopSleepTime)
	for {
		select {
		case <- t.C:
			resp, err := kapi.Get(context.Background(), Conf.Etcd.UDPTopicKey, nil)
			if err != nil {
				Log.Error("get udp topic from etcd failed, error = %v", err)
				continue
			}
			Log.Info("get udp topic %s", resp.Node.Value)
			if (KafkaInfo.UdpTopics == resp.Node.Value) {
				continue
			}
			Log.Warn("etcd udptopics changed from %s to %s", KafkaInfo.UdpTopics, resp.Node.Value)
			//fmt.Printf("etcd udptopics changed from %s to %s", KafkaInfo.UdpTopics, resp.Node.Value)
			KafkaInfo.UdpTopicsRWLock.Lock()
			KafkaInfo.UdpTopics = resp.Node.Value
			//拆分到map
			KafkaInfo.UdpTopicMap = make(map[string]bool)
			UTopics := strings.Split(resp.Node.Value, ",")
			for _, v := range UTopics {
				KafkaInfo.UdpTopicMap[v] = true
			}
			KafkaInfo.UdpTopicsRWLock.Unlock()

		case <-c.done:
			Log.Warn("watchUDPTopics stop")
			c.wg.Done()
			break
		}
	}
}

func (c *KafkaInfoKeeper) watchHTTPTopics() {
	kapi := client.NewKeysAPI(c.cli)
	t := time.NewTicker(loopSleepTime)
	for {
		select {
		case <- t.C:
			resp, err := kapi.Get(context.Background(), Conf.Etcd.HTTPTopicKey, nil)
			if err != nil {
				Log.Error("get http topic from etcd failed, error = %v", err)
				continue
			}
			Log.Info("get http topic %s", resp.Node.Value)
			if (KafkaInfo.HttpTopics == resp.Node.Value) {
				continue
			}
			Log.Warn("etcd udptopics changed from %s to %s", KafkaInfo.HttpTopics, resp.Node.Value)
			KafkaInfo.HttpTopicsRWLock.Lock()
			KafkaInfo.HttpTopics = resp.Node.Value
			HTopics := strings.Split(resp.Node.Value, ",")
			for _, v := range HTopics {
				KafkaInfo.HttpTopicMap[v] = true
			}
			KafkaInfo.HttpTopicsRWLock.Unlock()

		case <-c.done:
			Log.Warn("watchHTTPTopics stop")
			c.wg.Done()
			break
		}
	}
}
