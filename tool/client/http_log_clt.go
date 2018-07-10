/******************************************************
# DESC    :
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-03-24 15:49
# FILE    : http_log_clt.go
******************************************************/

package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
	"strings"
)

func main() {
	//url := "http://applogs.17paipai.cn/api/app/logs"
	url := "http://testapplogs.17paipai.cn/api/app/logs"

	payload := strings.NewReader("------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"pplog-biztype\"\r\n\r\ntopic-c-ad-click\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"pplog-log\"\r\n\r\n{\"Level\":\"104\",\"AdPlace\":\"3\",\"Version\":\"3.8.020\",\"AdType\":\"2\",\"TimeStamp\":\"1531121916\",\"DeviceID\":\"9008061B-1DEA-418F-B1EE-4E7FABDF0A35\",\"ChannelID\":\"Enterprise\",\"DeviceType\":\"2\",\"Sex\":\"1\",\"UserID\":\"9008000000022488\",\"AdID\":\"31\",\"City\":\"131\"}\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--")

	req, _ := http.NewRequest("POST", url, payload)

	req.Header.Add("content-type", "multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW")
	req.Header.Add("Cache-Control", "no-cache")
	req.Header.Add("Postman-Token", "2bd90188-654e-4d81-973d-cb3054535dce")

	res, _ := http.DefaultClient.Do(req)

	defer res.Body.Close()
	body, _ := ioutil.ReadAll(res.Body)

	fmt.Println(res)
	fmt.Println(string(body))

	return
}
