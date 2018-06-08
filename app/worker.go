/******************************************************
# DESC    : log kafka producer
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-03-22 20:48
# FILE    : worker.go
******************************************************/

package main

import (
	"fmt"
	"strings"
	"sync"
	"sync/atomic"
	"time"
)

import (
	"github.com/AlexStocks/goext/log/kafka"
	"github.com/AlexStocks/goext/runtime"
	"github.com/AlexStocks/goext/strings"
	"github.com/AlexStocks/goext/time"
	"github.com/Shopify/sarama"
)

/////////////////////////////////////////////////
// worker
/////////////////////////////////////////////////

type (
	Message struct {
		topic string
		key   []byte
		value []byte
	}

	KafkaWorker struct {
		Q    chan Message
		lock sync.Mutex
		done chan empty
		tick chan empty
		wg   sync.WaitGroup
	}

	MessageMetadata struct {
		EnqueuedAt time.Time
		Key        string
	}
)

func (mm *MessageMetadata) Latency() time.Duration {
	return time.Since(mm.EnqueuedAt)
}

func NewKafkaWorker() *KafkaWorker {
	return &KafkaWorker{
		done: make(chan empty),
		tick: make(chan empty),
	}
}



// Start for initialize all workers.
func (w *KafkaWorker) Start(workerNum int64, queueNum int64) {
	Log.Debug("worker number = %v, queue number is = %v", workerNum, queueNum)
	w.Q = make(chan Message, queueNum)

	KafkaInfo.workers = make(map[int64] bool)
	for i := int64(0); i < workerNum; i++ {
		KafkaInfo.workers[i] = true
	}
	for i := int64(0); i < workerNum; i++ {
		w.wg.Add(1)
		go w.startKafkaWorker(i)
	}
}

func (w *KafkaWorker) startKafkaWorker(workerIndex int64) {
	var (
		id              int
		kafkaProducerID string
		index           uint64
		successes       int
		failures        int
		err             error
		msgCallback     gxkafka.ProducerMessageCallback
		errCallback     gxkafka.ProducerErrorCallback
		producer        gxkafka.AsyncProducer
		message         Message
	)

	id = gxruntime.GoID()
	Log.Info("worker{%d-%d} starts to work now.", workerIndex, id)

	msgCallback = func(message *sarama.ProducerMessage) {
		var meta = message.Metadata.(MessageMetadata)
		if Conf.Core.Mode != "release" {
			Log.Info("send msg{%v} successfully, latency:%v, success num:%d, response{topic:%s, partition:%d, offset:%d}",
				meta.Key, meta.Latency(), successes, message.Topic, message.Partition, message.Offset)
			KafkaLog.Info("send msg{%v} successfully, latency:%v, success num:%d, response{topic:%s, partition:%d, offset:%d}",
				meta.Key, meta.Latency(), successes, message.Topic, message.Partition, message.Offset)
		}
		successes++
		StatStorage.AddKafkaSuccess(1)
		StatStorage.AddKafkaLatency(int64(meta.Latency()))
	}

	errCallback = func(err *sarama.ProducerError) {
		Log.Warn("send msg:%v failed, fail num:%d. error:%v", err.Msg, failures, err.Error())
		KafkaLog.Warn("send msg:%v failed, fail num:%d. error:%v", err.Msg, failures, err.Error())
		failures++
		StatStorage.AddKafkaError(1)
	}

	kafkaProducerID = fmt.Sprintf("%s-%d-%s-%d", LocalIP, Conf.Core.UDPPort, "telemetry", id)
	KafkaInfo.BrokersRWLock.RLock()
	producer, err = gxkafka.NewAsyncProducer(
		kafkaProducerID,
		strings.Split(KafkaInfo.Brokers, ","),
		gxkafka.HASH,
		false,
		45,
		sarama.CompressionSnappy,
		msgCallback,
		errCallback,
	)
	KafkaInfo.BrokersRWLock.RUnlock()
	if err != nil {
		panic(fmt.Sprintf("fail to gxkafka.NewProducer(id:%s, brokers:%s) = error:%v", id, Conf.Kafka.Brokers, err))
	}
	producer.Start()

LOOP:
	for {
		select {
		case message = <-w.Q:
			if Conf.Core.Mode != "release" {
				Log.Info("dequeue{worker{%d-%d} , message{topic:%s, key:%q, value:%q}}}",
					index, id, message.topic, gxstrings.String(message.key), gxstrings.String(message.value))
				KafkaLog.Info("dequeue{worker{%d-%d} , message{topic:%s, key:%q, value:%q}}}",
					index, id, message.topic, gxstrings.String(message.key), gxstrings.String(message.value))
			}
			producer.SendBytes(message.topic, message.key, message.value,
				MessageMetadata{EnqueuedAt: gxtime.Unix2Time(atomic.LoadInt64(&Now)), Key: gxstrings.String(message.key)})
			StatStorage.AddTotalCount(1)

		case <-w.tick:
			if KafkaInfo.workers[workerIndex] == false {
				producer.Stop()
				for {
					//restart produce
					KafkaInfo.BrokersRWLock.RLock()
					producer, err = gxkafka.NewAsyncProducer(
						kafkaProducerID,
						strings.Split(KafkaInfo.Brokers, ","),
						gxkafka.HASH,
						false,
						45,
						sarama.CompressionSnappy,
						msgCallback,
						errCallback,
					)
					KafkaInfo.BrokersRWLock.RUnlock()
					if err != nil {
						Log.Error("fail to restart kafka, brokers:%s, err = %v, wait retry...", KafkaInfo.Brokers, err)
						time.Sleep(time.Second * 1)
						continue
					}
					producer.Start()
					KafkaInfo.workers[workerIndex] = true
					Log.Warn("kafka reconnect to %s， workerIndex = %d", KafkaInfo.Brokers, workerIndex)
					break
				}
			}

		case <-w.done:
			producer.Stop()
			w.wg.Done()
			Log.Info("worker{%d-%d} exits now.", workerIndex, id)
			break LOOP
		}
	}
}

func (w *KafkaWorker) Stop() {
	close(w.done)
	w.wg.Wait()
}

func (w *KafkaWorker) Tick() {
	close(w.tick)
	w.tick = make(chan empty)
}

// queueNotification add kafka message to queue list.
func (w *KafkaWorker) enqueueKafkaMessage(message Message) {
	if Conf.Core.Mode != "release" {
		Log.Info("enqueue{Topic:%q, message{key:%q, value:%q}}",
			message.topic, gxstrings.String(message.key), gxstrings.String(message.value))
		KafkaLog.Info("enqueue{Topic:%q, message{key:%q, value:%q}}",
			message.topic, gxstrings.String(message.key), gxstrings.String(message.value))
	}
	w.Q <- message
}

func (w *KafkaWorker) BufLen() int {
	return len(w.Q)
}

func (w *KafkaWorker) Info() string {
	return fmt.Sprintf("kafka worker queue size %d", len(w.Q))
}
