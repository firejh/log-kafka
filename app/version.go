/******************************************************
# DESC       : version
# MAINTAINER : Alex Stocks
# LICENCE    : Apache License 2.0
# EMAIL      : alexstocks@foxmail.com
# MOD        : 2017-04-19 21:35
# FILE       : version.go
******************************************************/

package main

import (
	"fmt"
	"github.com/gin-gonic/gin"
	"runtime"
)

var (
	Version = "0.1.1"
	DATE    = "2018/03/22"
)

// SetVersion for setup Version string.
func SetVersion(ver string) {
	Version = ver
}

// PrintVersion provide print server engine
func PrintVersion() {
	fmt.Printf(`log-kafka %s, Compiler: %s %s, Copyright (C) %s Alex Stocks.`,
		Version,
		runtime.Compiler,
		runtime.Version(),
		DATE,
	)
	fmt.Println()
}

// VersionMiddleware : add Version on header.
func VersionMiddleware() gin.HandlerFunc {
	// Set out header value for each response
	return func(c *gin.Context) {
		c.Writer.Header().Set("Server-Version", "Worker/"+Version)
		c.Next()
	}
}
