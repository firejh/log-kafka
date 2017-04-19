package main

import (
	"fmt"
	"net"
	"sync"
	"time"
)

const (
	MaxMsgLen    = 32 * 1024
	ReadDeadline = 5e9
)

var (
	protocol = "udp"
)

type (
	UdpServer struct {
		conn *net.UDPConn
		done chan empty
		wg   sync.WaitGroup
	}
)

func NewUdpServer() *UdpServer {
	var (
		err     error
		addr    string
		udpAddr *net.UDPAddr
		server  *UdpServer
	)
	//Build the address
	addr = fmt.Sprintf(":%d", Conf.Core.UDPPort)
	if udpAddr, err = net.ResolveUDPAddr(protocol, addr); err != nil {
		panic(fmt.Sprintf("Wrong Address %q", addr))
	}

	server = &UdpServer{done: make(chan empty)}
	//Create the connection
	if server.conn, err = net.ListenUDP(protocol, udpAddr); err != nil {
		panic(fmt.Sprintf("net.ListenUDP(protocol:%s, udpAddr:%#v) = error:%#v", protocol, udpAddr, err))
	}

	server.wg.Add(1)
	go server.start()

	return server
}

func (u *UdpServer) start() {
	var (
		err      error
		ok       bool
		nerr     net.Error
		buf      []byte
		length   int
		seq      int
		peerAddr net.Addr
	)

	defer u.wg.Done()

	for {
		if u.isClosed() {
			Log.Warn("udp server exit now...")
			break
		}
		buf = make([]byte, MaxMsgLen)
		u.conn.SetReadDeadline(time.Now().Add(ReadDeadline))
		length, peerAddr, err = u.conn.ReadFromUDP(buf)
		if nerr, ok = err.(net.Error); ok && nerr.Timeout() {
			continue
		}
		if length == 0 || err != nil {
			Log.Warn("conn.ReadFromUDP() = {peer:%#v error:%#v}", peerAddr, err)
			continue
		}

		Worker.enqueueKafkaMessage(Message{key: []byte(fmt.Sprintf("%d", seq)), value: buf[:length]})
		seq++
	}
}

// check whether the server has been closed.
func (u *UdpServer) isClosed() bool {
	select {
	case <-u.done:
		return true

	default:
		return false
	}
}

func (u *UdpServer) Stop() {
	close(u.done)
	u.wg.Wait()
	u.conn.Close()
}
