package main

import (
	"net/http"
)

import (
	"github.com/gin-gonic/gin"
	api "gopkg.in/appleboy/gin-status-api.v1"
)

func rootHandler(c *gin.Context) {
	c.JSON(http.StatusOK, gin.H{
		"text": "Welcome to log kafka server.",
	})
}

func configHandler(c *gin.Context) {
	c.YAML(http.StatusCreated, Conf)
}

func routerEngine() *gin.Engine {
	// set server mode
	gin.SetMode(Conf.Core.Mode)

	r := gin.New()

	// Global middleware
	r.Use(gin.Logger())
	r.Use(gin.Recovery())
	r.Use(VersionMiddleware())
	r.Use(LogMiddleware())
	r.Use(StatMiddleware())

	r.GET(Conf.API.StatGoURI, api.StatusHandler)
	r.GET(Conf.API.StatAppURI, appStatusHandler)
	r.GET(Conf.API.ConfigURI, configHandler)
	r.GET(Conf.API.SysStatURI, sysStatsHandler)
	r.GET("/", rootHandler)
	r.POST(Conf.API.AppLogURI, appLogHandler)

	return r
}
