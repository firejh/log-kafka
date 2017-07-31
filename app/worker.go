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
	"github.com/AlexStocks/goext/time"
	"github.com/Shopify/sarama"
)

/////////////////////////////////////////////////
// worker
/////////////////////////////////////////////////

type (
	Message struct {
		key   []byte
		value []byte
	}

	KafkaWorker struct {
		Q    chan Message
		lock sync.Mutex
		done chan empty
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
	}
}

// Start for initialize all workers.
func (w *KafkaWorker) Start(workerNum int64, queueNum int64) {
	Log.Debug("worker number = %v, queue number is = %v", workerNum, queueNum)
	w.Q = make(chan Message, queueNum)
	for i := int64(0); i < workerNum; i++ {
		w.wg.Add(1)
		go w.startKafkaWorker()
	}
}

var (
	workerIndex uint64
)

func (w *KafkaWorker) startKafkaWorker() {
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
	index = atomic.AddUint64(&workerIndex, 1)
	Log.Info("worker{%d-%d} starts to work now.", index, id)

	msgCallback = func(message *sarama.ProducerMessage) {
		if Conf.Core.Mode != "release" {
			var meta = message.Metadata.(MessageMetadata)
			Log.Info("send msg{%v}, latency:%v, success num:%d, response{topic:%s, partition:%d, offset:%d}\n",
				meta.Key, meta.Latency(), successes, message.Topic, message.Partition, message.Offset)
		}
		successes++
	}

	errCallback = func(err *sarama.ProducerError) {
		Log.Warn("send msg:%v failed, fail num:%d. error:%v\n", err.Msg, failures, err.Error())
		failures++
	}

	kafkaProducerID = fmt.Sprintf("%s-%d-%s-%d", LocalIP, Conf.Core.UDPPort, "telemetry", id)
	producer, err = gxkafka.NewAsyncProducer(
		kafkaProducerID,
		strings.Split(Conf.Kafka.Brokers, ","),
		gxkafka.HASH,
		true,
		45,
		sarama.CompressionLZ4,
		msgCallback,
		errCallback,
	)
	if err != nil {
		panic(fmt.Sprintf("fail to gxkafka.NewProducer(id:%s, brokers:%s) = error:%v", id, Conf.Kafka.Brokers, err))
	}
	producer.Start()

LOOP:
	for {
		select {
		case message = <-w.Q:
			Log.Debug("dequeue{worker{%d-%d} , message{key:%q, value:%q}}}", index, id, string(message.key), string(message.value))
			producer.SendBytes(Conf.Kafka.Topic, message.key, message.value,
				MessageMetadata{EnqueuedAt: gxtime.Unix2Time(atomic.LoadInt64(&Now)), Key: string(message.key)})

		case <-w.done:
			producer.Stop()
			w.wg.Done()
			Log.Info("worker{%d-%d} exits now.", index, id)
			break LOOP
		}
	}
}

func (w *KafkaWorker) Stop() {
	close(w.done)
	w.wg.Wait()
}

// queueNotification add kafka message to queue list.
func (w *KafkaWorker) enqueueKafkaMessage(message Message) {
	Log.Debug("enqueue{message{key:%q, value:%q}}", string(message.key), string(message.value))
	w.Q <- message
}

func (w *KafkaWorker) Info() string {
	return fmt.Sprintf("kafka worker queue size %d", len(w.Q))
}
