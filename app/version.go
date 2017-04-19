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
	"runtime"
)

var (
	Version = "0.0.01"
	DATE    = "2017/04/19"
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
