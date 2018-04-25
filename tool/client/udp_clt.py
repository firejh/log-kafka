#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# ******************************************************
# DESC    :
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2017-04-18 15:08
# FILE    : udp_clt.py
# ******************************************************

#UDP client and server on localhost
import socket, sys, time
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# MAX = 65535
MAX = 2
PORT = 2000
LOOP = 1
# print 'Address before sending:', s.getsockname()
# msg="123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789abcdefghijklmnopqrstuvwxyz123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789"
msg="123456789"

def client1():
    start = time.time()
    s.settimeout(5)
    for i in range(LOOP):
        s.sendto('This is my message', ('127.0.0.1', PORT))
        print 'idx:%s, msg:%s' % (i, msg)
        # print 'Address after sending', s.getsockname()
        # data, address = s.recvfrom(MAX) # overly promiscuous - see text!
        # print 'The server', address, 'says', repr(data)

    print 'cost:', time.time() - start

def client():
    """
    settimeout(timeout)

    Set a timeout on socket operations.  'timeout' can be a float,
    giving in seconds, or None.  Setting a timeout of None disables
    the timeout feature and is equivalent to setblocking(1).
    Setting a timeout of zero is the same as setblocking(0).
    """
    s.settimeout(5)
    s.connect(('127.0.0.1', PORT))
    s.settimeout(None)
    # s.settimeout(5)
    start = time.time()
    print 'msg len:', len(msg)
    for i in range(LOOP):
        s.send(msg)
        # print 'Address after sending', s.getsockname()
        # data = s.recv(MAX) # overly promiscuous - see text!
        # print 'says', repr(data)

    print 'cost:', time.time() - start

def input_echo():
    while True:
        data = raw_input('Input msg: ')
        if len(data) == 0:
            continue
        s.sendto(data.encode(), ('127.0.0.1', 3000))
        print (s.recv(1024).decode('utf-8'))

if __name__ == "__main__":
    # client()
    # 由于log-kafka是面向无连接的，所以使用client1会减少丢包
    client1()
    s.close()