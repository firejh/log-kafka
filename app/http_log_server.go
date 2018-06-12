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
	"github.com/AlexStocks/goext/database/registry"
	"github.com/AlexStocks/goext/strings"
	"github.com/gin-gonic/gin"
	"github.com/gin-gonic/gin/json"
	"github.com/juju/errors"
)

const (
	httpStatusIllegalParam = 460
	httpStatusServerFail   = 520
)

func getLogServersHandler(c *gin.Context) {
	var (
		err         error
		bizType     string
		bizVersion  string
		bizProtocol string
		clientAddr  string
		attr        gxregistry.ServiceAttr
		services    []*gxregistry.Service
		result      gxregistry.Service
		jsonBytes   []byte
	)

	clientAddr = c.GetHeader(textproto.CanonicalMIMEHeaderKey("X-Forwarded-For"))
	HTTPLog.Debug("clientAddr:%+v", clientAddr)

	bizType = c.PostForm(logBizType)
	bizVersion = c.PostForm(logBizVersion)
	bizProtocol = c.PostForm(logBizProtocol)
	HTTPLog.Debug("bizType:%+v, bizVersion:%+v, bizProtocol:%+v", bizType, bizVersion, bizProtocol)
	if len(bizType) == 0 {
		c.JSON(httpStatusIllegalParam, gin.H{
			"status":  httpStatusIllegalParam,
			"message": logBizType + " is nil",
		})
		StatStorage.AddHttpError(1)
		HTTPLog.Warn("client:%q, bizType is nil", clientAddr)
		return
	}

	attr = gxregistry.ServiceAttr{
		Service:  bizType,
		Version:  bizVersion,
		Role:     gxregistry.SRT_Provider,
		Protocol: bizProtocol,
	}

	services, err = Filter.GetService(attr)
	if err != nil || len(services) == 0 {
		c.JSON(httpStatusIllegalParam, gin.H{
			"status":  httpStatusServerFail,
			"message": err.Error(),
		})
		StatStorage.AddHttpError(1)
		HTTPLog.Warn("Filter.GetService(attr:%+v) = error:%q, services:%+v", attr, err, services)
		return
	}

	HTTPLog.Debug("services:%+v", services)
	result.Attr = &attr
	for i := range services {
		for j := range services[i].Nodes {
			result.Nodes = append(result.Nodes, services[i].Nodes[j])
		}
	}
	jsonBytes, err = json.Marshal(result)
	if err != nil {
		c.JSON(httpStatusIllegalParam, gin.H{
			"status":  httpStatusServerFail,
			"message": err.Error(),
		})
		StatStorage.AddHttpError(1)
		HTTPLog.Warn("json.Marshal(result:%+v) = error:%q", result, err)
		return
	}
	HTTPLog.Debug("services:%+v, result:%+v", services, result)

	c.JSON(http.StatusOK, gin.H{
		"status":  http.StatusOK,
		"service": string(jsonBytes),
	})
	StatStorage.AddHttpSuccess(1)
}

func appLogHandler(c *gin.Context) {
	var (
		err                                   error
		index                                 int
		bizType, zipType, logData, appLogData string
		unzipBytes                            []byte
		logKey                                []byte
		clientAddr                            string
		lines                                 []string
	)

	clientAddr = c.GetHeader(textproto.CanonicalMIMEHeaderKey("X-Forwarded-For"))
	HTTPLog.Debug("clientAddr:%+v", clientAddr)

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
	HTTPLog.Debug("bizType:%+v", bizType)

	//if !gxstrings.Contains(Conf.Kafka.HTTPTopics, bizType) {
	if _, ok := KafkaInfo.HttpTopicMap[bizType]; !ok {
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
	HTTPLog.Debug("zipType:%v, logData:%v", zipType, logData)
	if len(logData) == 0 {
		c.JSON(httpStatusIllegalParam, gin.H{
			"status":  httpStatusIllegalParam,
			"message": logText + " is nil",
		})
		StatStorage.AddHttpError(1)
		HTTPLog.Warn("client:%q, logData is nil", clientAddr)
		return
	}

	err = nil
	switch zipType {
	case "zip":
		if unzipBytes, err = gxzlib.DoZlibUncompress(gxstrings.Slice(logData)); err != nil {
			HTTPLog.Error("zipType:zip, gxzlib.DoZlibUncompress() = error:%s", errors.ErrorStack(err))
			// if unzipBytes, err = gxgzip.DoGzipUncompress(gxstrings.Slice(logData)); err != nil {
			//	ioutil.WriteFile("./log.zip", gxstrings.Slice(logData), 0644)
			// HTTPLog.Error("zipType:zip, gxzlib.DoGzipUncompress() = error:%s", errors.ErrorStack(err))
			c.JSON(httpStatusIllegalParam, gin.H{
				"status":  httpStatusIllegalParam,
				"message": "can not uncompress log",
			})
			StatStorage.AddHttpError(1)
			return
			// }
		}
		appLogData = gxstrings.String(unzipBytes)

	case "gzip":
		if unzipBytes, err = gxgzip.DoGzipUncompress(gxstrings.Slice(logData)); err != nil {
			HTTPLog.Error("zipType:gzip, gxzlib.DoGzipUncompress() = error:%s", errors.ErrorStack(err))
			// if unzipBytes, err = gxzlib.DoZlibUncompress(gxstrings.Slice(logData)); err != nil {
			//	ioutil.WriteFile("./log.gzip", gxstrings.Slice(logData), 0644)
			// HTTPLog.Error("zipType:gzip, gxzlib.DoZlibUncompress() = error:%s", errors.ErrorStack(err))
			c.JSON(httpStatusIllegalParam, gin.H{
				"status":  httpStatusIllegalParam,
				"message": "can not uncompress log",
			})
			StatStorage.AddHttpError(1)
			return
			// }
		}
		appLogData = gxstrings.String(unzipBytes)

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
		HTTPLog.Debug("client:%q, log:{topic:%q, value:%s}",
			clientAddr, bizType, lines[index])
		fmt.Printf("client:%q, log:{topic:%q, value:%s}\n",
			clientAddr, bizType, lines[index])
	}

	c.JSON(http.StatusOK, gin.H{
		"status": http.StatusOK,
	})
	StatStorage.AddHttpSuccess(1)
}
