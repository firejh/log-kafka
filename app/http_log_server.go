/******************************************************
# DESC    : http handler
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-03-23 16:04
# FILE    : http_log_server.go
******************************************************/

package main

import (
	"fmt"
	"net/http"
	"net/textproto"
	"strings"
)

import (
	"github.com/AlexStocks/goext/compress/gzip"
	"github.com/AlexStocks/goext/compress/zlib"
	"github.com/AlexStocks/goext/strings"
	"github.com/gin-gonic/gin"
)

const (
	httpStatusIllegalParam = 460
)

func appLogHandler(c *gin.Context) {
	var (
		index                                 int
		bizType, zipType, logData, appLogData string
		logKey                                []byte
		clientAddr                            string
		lines                                 []string
	)

	clientAddr = c.GetHeader(textproto.CanonicalMIMEHeaderKey("X-Forwarded-For"))

	bizType = c.PostForm(logBizType)
	if len(bizType) == 0 {
		c.JSON(httpStatusIllegalParam, gin.H{
			"status":  httpStatusIllegalParam,
			"message": logBizType + " is nil",
		})
		StatStorage.AddHttpError(1)
		HTTPLog.Warn("client:%q, bizType is nil", clientAddr)
		return
	}

	if !gxstrings.Contains(Conf.Kafka.HTTPTopics, bizType) {
		c.JSON(httpStatusIllegalParam, gin.H{
			"status":  httpStatusIllegalParam,
			"message": bizType + " not exist",
		})
		StatStorage.AddHttpError(1)
		HTTPLog.Warn("client:%q, bizType %s not exist", clientAddr, bizType)
		return
	}

	zipType = c.PostForm(logZipType)
	logData = c.PostForm(logText)
	if len(logData) == 0 {
		c.JSON(httpStatusIllegalParam, gin.H{
			"status":  httpStatusIllegalParam,
			"message": logText + " is nil",
		})
		StatStorage.AddHttpError(1)
		HTTPLog.Warn("client:%q, logData is nil", clientAddr)
		return
	}

	switch zipType {
	case "zip":
		appLogData = gxstrings.String(gxzlib.DoZlibUncompress(gxstrings.Slice(logData)))

	case "gzip":
		appLogData = gxstrings.String(gxgzip.DoGzipUncompress(gxstrings.Slice(logData)))

	default:
		appLogData = logData
	}
	HTTPLog.Debug("zipType:%s, appLogData:%s", zipType, appLogData)
	if appLogData == "" {
		c.JSON(httpStatusIllegalParam, gin.H{
			"status":  httpStatusIllegalParam,
			"message": "appLogData is nil",
		})
		StatStorage.AddHttpError(1)
		HTTPLog.Warn("client:%q, appLogData is nil")
		return
	}

	// 使用不变的key，尽量使得这批log以batch方式快速塞入kafka
	if len(logData) > 16 {
		logKey = gxstrings.Slice(logData[:16])
	} else {
		logKey = gxstrings.Slice(fmt.Sprintf("%d", StatStorage.GetHttpSuccess()))
	}
	lines = strings.Split(appLogData, "\n")
	for index = range lines {
		Worker.enqueueKafkaMessage(Message{
			topic: bizType,
			key:   logKey,
			value: gxstrings.Slice(lines[index]),
		})
		HTTPLog.Debug("client:%q, log:{topic:%q, key:%d, value:%s}",
			clientAddr, bizType, gxstrings.String(logKey), lines[index])
	}

	c.JSON(http.StatusOK, gin.H{
		"status": http.StatusOK,
	})
	StatStorage.AddHttpSuccess(1)
}
