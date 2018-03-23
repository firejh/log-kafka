/******************************************************
# DESC    : test http handler
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-03-22 20:45
# FILE    : http_server_test.go
******************************************************/

package main

import (
	"net/http"
	"runtime"
	"testing"
	"time"
)

import (
	"github.com/buger/jsonparser"
	"github.com/gin-gonic/gin"
	"github.com/stretchr/testify/assert"
	"gopkg.in/appleboy/gofight.v2"
)

var goVersion = runtime.Version()

func initTest() {
	PushConf = BuildDefaultPushConf()
	PushConf.Core.Mode = "test"
}

func TestPrintGoRushVersion(t *testing.T) {
	SetVersion("3.0.0")
	ver := GetVersion()
	PrintWorkerVersion()

	assert.Equal(t, "3.0.0", ver)
}

func TestRunNormalServer(t *testing.T) {
	initTest()

	gin.SetMode(gin.TestMode)

	go func() {
		assert.NoError(t, initHTTPServer())
	}()
	// have to wait for the goroutine to start and run the server
	// otherwise the main thread will complete
	time.Sleep(5 * time.Millisecond)

	assert.Error(t, initHTTPServer())
	gofight.TestRequest(t, "http://localhost:8088/api/stat/go")
}

func TestRootHandler(t *testing.T) {
	initTest()

	r := gofight.New()

	// log for json
	PushConf.Log.LogFormat = "json"

	r.GET("/").
		Run(routerEngine(), func(r gofight.HTTPResponse, rq gofight.HTTPRequest) {
			data := []byte(r.Body.String())

			value, _ := jsonparser.GetString(data, "text")

			assert.Equal(t, "Welcome to pusher worker server.", value)
			assert.Equal(t, http.StatusOK, r.Code)
		})
}

func TestAPIStatusGoHandler(t *testing.T) {
	initTest()

	r := gofight.New()

	r.GET("/api/stat/go").
		Run(routerEngine(), func(r gofight.HTTPResponse, rq gofight.HTTPRequest) {
			data := []byte(r.Body.String())

			value, _ := jsonparser.GetString(data, "go_version")

			assert.Equal(t, goVersion, value)
			assert.Equal(t, http.StatusOK, r.Code)
		})
}

func TestAPIStatusAppHandler(t *testing.T) {
	initTest()

	r := gofight.New()

	appVersion := "v1.0.0"
	SetVersion(appVersion)

	r.GET("/api/stat/app").
		Run(routerEngine(), func(r gofight.HTTPResponse, rq gofight.HTTPRequest) {
			data := []byte(r.Body.String())

			value, _ := jsonparser.GetString(data, "version")

			assert.Equal(t, appVersion, value)
			assert.Equal(t, http.StatusOK, r.Code)
		})
}

func TestAPIConfigHandler(t *testing.T) {
	initTest()

	r := gofight.New()

	r.GET("/api/config").
		Run(routerEngine(), func(r gofight.HTTPResponse, rq gofight.HTTPRequest) {
			assert.Equal(t, http.StatusCreated, r.Code)
		})
}

func TestSysStatsHandler(t *testing.T) {
	initTest()

	r := gofight.New()

	r.GET("/sys/stats").
		Run(routerEngine(), func(r gofight.HTTPResponse, rq gofight.HTTPRequest) {
			assert.Equal(t, http.StatusOK, r.Code)
		})
}
