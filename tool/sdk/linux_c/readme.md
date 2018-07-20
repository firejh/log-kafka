# readme
---

log-kafka 的 C 语言客户端：定时向 log-kafka 发起 HTTP 请求和读取其响应，获取 log-kafka 节点列表；轮询异步地向 log-kafka 发起 udp 日志写请求。

# third list
---

> 2018/07/19
 * [pbjson](https://github.com/yinqiwen/pbjson)===master，作用是把 log-kafka 通过 HTTP 返回的响应内容由 json 格式转换为 protobuf 结构体；
 * rapidjson，pbjson 依赖库；
 * protobuf===2.3.5， pbjson 依赖库；
 * curl===7.57.0，用于向 log-kafka 发起 HTTP 请求和读取其响应；
