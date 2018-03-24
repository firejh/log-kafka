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
	"bytes"
	"compress/gzip"
	"compress/zlib"
	"fmt"
	"io"
	"strings"
)

import (
	"github.com/AlexStocks/goext/strings"
	"github.com/gin-gonic/gin"
	"net/http"
	"net/textproto"
)

func uncompressZipText(text []byte) string {
	b := bytes.NewReader(text)
	var out bytes.Buffer
	r, _ := zlib.NewReader(b)
	io.Copy(&out, r)
	return out.String()
}

func uncompressGzipText(text []byte) string {
	b := bytes.NewReader(text)
	var out bytes.Buffer
	r, _ := gzip.NewReader(b)
	io.Copy(&out, r)
	return out.String()
}

func appLogHandler(c *gin.Context) {
	var (
		index, key                            int
		bizType, zipType, logData, appLogData string
		clientAddr                            string
		lines                                 []string
	)

	clientAddr = c.GetHeader(textproto.CanonicalMIMEHeaderKey("X-Forwarded-For"))

	bizType = c.PostForm(logBizType)
	if len(bizType) == 0 {
		c.JSON(460, gin.H{
			"status":  "failed",
			"message": logBizType + " is nil",
		})
		StatStorage.AddHttpError(1)
		HTTPLog.Warn("client:%q, bizType is nil", clientAddr)
		return
	}

	zipType = c.PostForm(logZipType)
	logData = c.PostForm(logText)
	if len(logData) == 0 {
		c.JSON(460, gin.H{
			"status":  "failed",
			"message": logText + " is nil",
		})
		StatStorage.AddHttpError(1)
		HTTPLog.Warn("client:%q, logData is nil", clientAddr)
		return
	}

	switch zipType {
	case "zip":
		appLogData = uncompressZipText(gxstrings.Slice(logData))

	case "gzip":
		appLogData = uncompressGzipText(gxstrings.Slice(logData))

	default:
		appLogData = logData
	}

	lines = strings.Split(appLogData, "\n")
	key = int(StatStorage.GetHttpSuccess())
	for index = range lines {
		Worker.enqueueKafkaMessage(Message{
			topic: bizType,
			key:   []byte(fmt.Sprintf("%d", key)),
			value: gxstrings.Slice(lines[index]),
		})
		HTTPLog.Debug("client:%q, log:{topic:%q, key:%d, value:%s}", clientAddr, key, lines[index])
		key++
	}

	c.JSON(http.StatusOK, gin.H{
		"status": "success",
	})
	StatStorage.AddHttpSuccess(1)
}
