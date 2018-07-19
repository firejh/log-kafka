# readme
---

the third open source project.

# list
---

> 2018/07/19
 * client_sdk, 代码源自 log-kafka/sdk/linux_c，作用是定时向 log-kafka 发起 HTTP 请求和读取其响应，获取 log-kafka 节点列表；向 log-kafka 发起 udp 日志写请求；
 * protobuf===2.3.5，log-kafka/sdk/linux_c 所依赖的库，为 pbjson 库所调用；
 * curl===7.57.0，log-kafka/sdk/linux_c 所依赖的库，用于向 log-kafka 发起 HTTP 请求和读取其响应；
 * iniparser，用于读取 ini 配置；
 * uds_datagram，用于收取 unixsocket datagram 包，包中包含了需要经过 log-kafka 写入 kafka中的日志内容；
 * glog，log 库。
