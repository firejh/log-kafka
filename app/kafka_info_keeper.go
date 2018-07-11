package main

import (
	"context"
	"strings"
	"sync"
	"time"
	"fmt"
)

import (
	"github.com/coreos/etcd/clientv3"
)

type (
	EtcdKafkaInfo struct {
		Key              string
		Brokers          string
		BrokersRWLock    sync.RWMutex
		UdpTopics        string
		UdpTopicMap      map[string]bool
		UdpTopicsRWLock  sync.RWMutex
		HttpTopics       string
		HttpTopicMap     map[string]bool
		HttpTopicsRWLock sync.RWMutex
		workers          map[int64]bool //运行中map的key初始化后不会变动，只有对应的value可能变化，不用加锁不需要原子操作
	}

	KafkaInfoKeeper struct {
		client *clientv3.Client
		st     bool
		wg     sync.WaitGroup
		done   chan empty
	}
)

var (
	//etcd kafka info
	KafkaInfo      EtcdKafkaInfo
	dialTimeout    = 5 * time.Second
	requestTimeout = 5 * time.Second
	loopSleepTime  = 5 * time.Second
)

func NewKafkaInfoKeeper() (*KafkaInfoKeeper, error) {
	var (
		server *KafkaInfoKeeper
		err    error
	)

	server = &KafkaInfoKeeper{
		st:   true,
		done: make(chan empty),
	}

	server.client, err = clientv3.New(clientv3.Config{
		Endpoints:   Conf.Etcd.Addrs,
		DialTimeout: dialTimeout,
	})
	if err != nil {
		return nil, err
	}

	return server, nil
}

func (c *KafkaInfoKeeper) Start() error {
	ctx, cancel := context.WithTimeout(context.Background(), requestTimeout)
	resp, err := c.client.Get(ctx, Conf.Etcd.KafkaInfoKey)
	cancel()
	if err != nil {
		return err
	}
	if len(resp.Kvs) > 0 {
		KafkaInfo.Brokers = string(resp.Kvs[0].Value)
	}

	//udp topic, http topic init
	ctx, cancel = context.WithTimeout(context.Background(), requestTimeout)
	resp, err = c.client.Get(ctx, Conf.Etcd.UDPTopicKey)
	cancel()
	if err != nil {
		return err
	}
	if len(resp.Kvs) > 0 {
		KafkaInfo.UdpTopics = string(resp.Kvs[0].Value)
	}
	KafkaInfo.UdpTopicMap = make(map[string]bool)
	UTopics := strings.Split(KafkaInfo.UdpTopics, ",")
	for _, v := range UTopics {
		KafkaInfo.UdpTopicMap[v] = true
	}

	ctx, cancel = context.WithTimeout(context.Background(), requestTimeout)
	resp, err = c.client.Get(ctx, Conf.Etcd.HTTPTopicKey)
	cancel()
	if err != nil {
		return err
	}
	if len(resp.Kvs) > 0 {
		KafkaInfo.HttpTopics = string(resp.Kvs[0].Value)
	}
	KafkaInfo.HttpTopicMap = make(map[string]bool)
	HTopics := strings.Split(KafkaInfo.HttpTopics, ",")
	for _, v := range HTopics {
		KafkaInfo.HttpTopicMap[v] = true
	}

	fmt.Printf("kafka_info : %v", KafkaInfo)

	//watch kafka info
	c.wg.Add(1)
	go c.watchKafkaPath()

	return nil
}

func (c *KafkaInfoKeeper) Stop() {
	select {
	case <-c.done:
		return
	default:
		close(c.done)
		c.client.Close()
	}

	c.wg.Wait()
}

func (c *KafkaInfoKeeper) Closed() bool {
	select {
	case <-c.done:
		return true

	default:
		return false
	}
}

func (c *KafkaInfoKeeper) watchKafkaPath() {
	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	watchPath := Conf.Etcd.KafkaClusterPath
	if !strings.HasSuffix(watchPath, "/") {
		watchPath += "/"
	}
	rCh := c.client.Watch(ctx, watchPath, clientv3.WithPrefix(), clientv3.WithPrevKV())
	for wResp := range rCh {
		if c.Closed() {
			break
		}
		for _, ev := range wResp.Events {
			if len(ev.Kv.Value) == 0 {
				continue
			}
			switch ev.Type {
			case clientv3.EventTypePut:
				switch string(ev.Kv.Key) {
				case Conf.Etcd.KafkaInfoKey:
					go c.updateKafkaInfo(string(ev.Kv.Value))
				case Conf.Etcd.UDPTopicKey:
					go c.updateUDPTopic(string(ev.Kv.Value))
				case Conf.Etcd.HTTPTopicKey:
					go c.updateHTTPTopic(string(ev.Kv.Value))
				default:
					Log.Warn("unexpected etcd key, watch path = %s, key = %s", Conf.Etcd.KafkaClusterPath, string(ev.Kv.Key))
				}
			default:
				Log.Warn("unexpected etcd watch type = %d", ev.Type)
			}
		}
	}
	Log.Warn("watchKafkaPath goroutine stopped")
}

func (c *KafkaInfoKeeper) updateKafkaInfo(value string) {
	Log.Warn("etcd kafka info changed from %s to %s", KafkaInfo.Brokers, value)
	KafkaInfo.BrokersRWLock.Lock()
	KafkaInfo.Brokers = value
	for k := range KafkaInfo.workers {
		KafkaInfo.workers[k] = false
	}
	KafkaInfo.BrokersRWLock.Unlock()
}

func (c *KafkaInfoKeeper) updateUDPTopic(value string) {
	Log.Warn("etcd udptopics changed from %s to %s", KafkaInfo.UdpTopics, value)
	KafkaInfo.UdpTopicsRWLock.Lock()
	KafkaInfo.UdpTopics = value
	//拆分到map
	KafkaInfo.UdpTopicMap = make(map[string]bool)
	UTopics := strings.Split(value, ",")
	for _, v := range UTopics {
		KafkaInfo.UdpTopicMap[v] = true
	}
	KafkaInfo.UdpTopicsRWLock.Unlock()
}

func (c *KafkaInfoKeeper) updateHTTPTopic(value string) {
	Log.Warn("etcd udptopics changed from %s to %s", KafkaInfo.HttpTopics, value)
	KafkaInfo.HttpTopicsRWLock.Lock()
	KafkaInfo.HttpTopics = value
	HTopics := strings.Split(value, ",")
	for _, v := range HTopics {
		KafkaInfo.HttpTopicMap[v] = true
	}
	KafkaInfo.HttpTopicsRWLock.Unlock()
}
