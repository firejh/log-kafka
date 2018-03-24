#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# ******************************************************
# DESC    :
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-03-24 15:44
# FILE    : http_log_clt.py
# ******************************************************

'''
POST /api/app/logs HTTP/1.1
Host: localhost:3000
Cache-Control: no-cache
Postman-Token: f7df606c-87c5-4f6a-9e0d-7c7603346529
Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW

------WebKitFormBoundary7MA4YWxkTrZu0gW
Content-Disposition: form-data; name="pplog-biztype"

http1
------WebKitFormBoundary7MA4YWxkTrZu0gW
Content-Disposition: form-data; name="pplog-log"

hello5
hello6
hello7
------WebKitFormBoundary7MA4YWxkTrZu0gW--
'''

import requests

url = "http://localhost:3000/api/app/logs"

payload = "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"pplog-biztype\"\r\n\r\nhttp1\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"pplog-log\"\r\n\r\nhello5\nhello6\nhello7\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--"
headers = {
    'content-type': "multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW",
    'Cache-Control': "no-cache",
    'Postman-Token': "443337d1-66a8-435e-b595-32e94943f5dd"
    }

response = requests.request("POST", url, data=payload, headers=headers)

print(response.text)
