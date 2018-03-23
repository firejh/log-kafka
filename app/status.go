package main

import (
	"net/http"
	"sync/atomic"
)

import (
	"github.com/gin-gonic/gin"
	"github.com/thoas/stats"
)

////////////////////////////////////////////////
// Storage
////////////////////////////////////////////////

// StatusApp is app status structure
// 相关字段都是atomic操作，所以是线程安全的
type statApp struct {
	TotalCount int64       `json:"total_count"`
	Kafka      KafkaStatus `json:"kafka"`
	Udp        UDPStatus   `json:"udp"`
	Http       HTTPStatus  `json:"http"`
}

// Storage is interface structure
type Storage struct {
	stat *statApp
}

// Init client storage.
func (s *Storage) Init() error {
	return nil
}

// New func implements the storage interface
func NewStorage() *Storage {
	return &Storage{
		stat: &statApp{},
	}
}

// Reset Client storage.
func (s *Storage) Reset() {
	atomic.StoreInt64(&s.stat.TotalCount, 0)
	atomic.StoreInt64(&s.stat.Kafka.LogKafkaSuccess, 0)
	atomic.StoreInt64(&s.stat.Kafka.LogKafkaError, 0)
	atomic.StoreInt64(&s.stat.Udp.LogKafkaSuccess, 0)
	atomic.StoreInt64(&s.stat.Udp.LogKafkaError, 0)
	atomic.StoreInt64(&s.stat.Http.LogKafkaSuccess, 0)
	atomic.StoreInt64(&s.stat.Http.LogKafkaError, 0)
}

// AddTotalCount record log request count.
func (s *Storage) AddTotalCount(count int64) {
	atomic.AddInt64(&s.stat.TotalCount, count)
}

// AddKafkaSuccess record counts of success kafka log request.
func (s *Storage) AddKafkaSuccess(count int64) {
	atomic.AddInt64(&s.stat.Kafka.LogKafkaSuccess, count)
}

// AddKafkaError record counts of error kafka log request.
func (s *Storage) AddKafkaError(count int64) {
	atomic.AddInt64(&s.stat.Kafka.LogKafkaError, count)
}

// AddUdpSuccess record counts of success iOS log request.
func (s *Storage) AddUdpSuccess(count int64) {
	atomic.AddInt64(&s.stat.Udp.LogKafkaSuccess, count)
}

// AddUdpError record counts of error iOS log request.
func (s *Storage) AddUdpError(count int64) {
	atomic.AddInt64(&s.stat.Udp.LogKafkaError, count)
}

// AddHttpSuccess record counts of success Http log request.
func (s *Storage) AddHttpSuccess(count int64) {
	atomic.AddInt64(&s.stat.Http.LogKafkaSuccess, count)
}

// AddHttpError record counts of error Http log request.
func (s *Storage) AddHttpError(count int64) {
	atomic.AddInt64(&s.stat.Http.LogKafkaError, count)
}

// GetTotalCount show counts of all notification.
func (s *Storage) GetTotalCount() int64 {
	count := atomic.LoadInt64(&s.stat.TotalCount)

	return count
}

// GetKafkaSuccess show success counts of kafka log requests
func (s *Storage) GetKafkaSuccess() int64 {
	count := atomic.LoadInt64(&s.stat.Kafka.LogKafkaSuccess)

	return count
}

// GetKafkaError show error counts of kafka log requests
func (s *Storage) GetKafkaError() int64 {
	count := atomic.LoadInt64(&s.stat.Kafka.LogKafkaError)

	return count
}

// GetUdpSuccess show success counts of udp log requests
func (s *Storage) GetUdpSuccess() int64 {
	count := atomic.LoadInt64(&s.stat.Udp.LogKafkaSuccess)

	return count
}

// GetUdpError show error counts of udp log requests
func (s *Storage) GetUdpError() int64 {
	count := atomic.LoadInt64(&s.stat.Udp.LogKafkaError)

	return count
}

// GetHttpSuccess show success counts of Http notification.
func (s *Storage) GetHttpSuccess() int64 {
	count := atomic.LoadInt64(&s.stat.Http.LogKafkaSuccess)

	return count
}

// GetHttpError show error counts of Http notification.
func (s *Storage) GetHttpError() int64 {
	count := atomic.LoadInt64(&s.stat.Http.LogKafkaError)

	return count
}

////////////////////////////////////////////////
// http server
////////////////////////////////////////////////

// Stats provide response time, status code count, etc.
var Stats = stats.New()

// StatusApp is app status structure
type StatusApp struct {
	Version    string      `json:"version"`
	TotalCount int64       `json:"total_count"`
	Kafka      KafkaStatus `json:"kafka"`
	Udp        UDPStatus   `json:"udp"`
	Http       HTTPStatus  `json:"http"`
}

// KafkaStatus is kafka structure
type KafkaStatus struct {
	LogKafkaSuccess int64 `json:"log_kafka_success"`
	LogKafkaError   int64 `json:"log_kafka_error"`
}

// UDPStatus is udp structure
type UDPStatus struct {
	LogKafkaSuccess int64 `json:"log_kafka_success"`
	LogKafkaError   int64 `json:"log_kafka_error"`
}

// HTTPStatus is http structure
type HTTPStatus struct {
	LogKafkaSuccess int64 `json:"log_kafka_success"`
	LogKafkaError   int64 `json:"log_kafka_error"`
}

// initAppStatus for initialize app status
func initAppStatus() error {
	var err error

	StatStorage = NewStorage()
	if err = StatStorage.Init(); err != nil {
		Log.Error("storage error: " + err.Error())
	}

	return err
}

func appStatusHandler(c *gin.Context) {
	result := StatusApp{
		Version:    Version,
		TotalCount: StatStorage.GetTotalCount(),
		Kafka: KafkaStatus{
			LogKafkaSuccess: StatStorage.GetKafkaSuccess(),
			LogKafkaError:   StatStorage.GetKafkaError(),
		},
		Udp: UDPStatus{
			LogKafkaSuccess: StatStorage.GetUdpSuccess(),
			LogKafkaError:   StatStorage.GetUdpError(),
		},
		Http: HTTPStatus{
			LogKafkaSuccess: StatStorage.GetHttpSuccess(),
			LogKafkaError:   StatStorage.GetHttpError(),
		},
	}

	c.JSON(http.StatusOK, result)
}

func sysStatsHandler(c *gin.Context) {
	c.JSON(http.StatusOK, Stats.Data())
}

// StatMiddleware response time, status code count, etc.
func StatMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		beginning, recorder := Stats.Begin(c.Writer)
		c.Next()
		Stats.End(beginning, recorder)
	}
}
