// +build darwin dragonfly freebsd linux nacl netbsd openbsd solaris

/******************************************************
# DESC    : http server
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-03-22 20:44
# FILE    : http_server_unix.go
******************************************************/

package main

import (
	"fmt"
	"net/http"
)

import (
	"github.com/facebookgo/grace/gracehttp"
)

// initHTTPServer provide run http or https protocol.
func initHTTPServer() {
	addr := fmt.Sprintf(":%d", Conf.Core.HTTPPort)
	go gracehttp.Serve(&http.Server{
		Addr:    addr,
		Handler: routerEngine(),
	})
}
