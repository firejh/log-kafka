/******************************************************
# DESC    : configure
# AUTHOR  : Alex Stocks
# VERSION : 1.0
# LICENCE : Apache License 2.0
# EMAIL   : alexstocks@foxmail.com
# MOD     : 2018-03-22 20:43
# FILE    : config.go
******************************************************/

package main

import (
	"io/ioutil"
)

import (
	"gopkg.in/yaml.v2"
)

// ConfYaml is config structure.
type ConfYaml struct {
	Core  SectionCore  `yaml:"core"`
	API   SectionAPI   `yaml:"api"`
	Kafka SectionKafka `yaml:"kafka"`
	Etcd  SectionEtcd  `yaml:"etcd"`
}

// SectionPID is sub section of config.
type SectionPID struct {
	Enabled  bool   `yaml:"enabled"`
	Path     string `yaml:"path"`
	Override bool   `yaml:"override"`
}

// SectionCore is sub section of config.
type SectionCore struct {
	Mode            string     `yaml:"mode"`
	FailFastTimeout int        `yaml:"fail_fast_timeout"`
	WorkerNum       int64      `yaml:"worker_num"`
	QueueNum        int64      `yaml:"queue_num"`
	PID             SectionPID `yaml:"pid"`
	LocalIP         string     `yaml:"local_ip"`
	UDPPort         int        `yaml:"udp_port"`
	UDPReadBufSize  int        `yaml:"udp_read_buffer_size"`
	UDPReadTimeout  int        `yaml:"udp_read_timeout"`
	HTTPPort        int        `yaml:"http_port"`
	LogSize         int        `yaml:"log_size"`
}

// SectionAPI is sub section of config.
type SectionAPI struct {
	StatGoURI  string `yaml:"stat_go_uri"`
	StatAppURI string `yaml:"stat_app_uri"`
	ConfigURI  string `yaml:"config_uri"`
	SysStatURI string `yaml:"sys_stat_uri"`
	AppLogURI  string `yaml:"app_log_uri"`
}

// SectionKafka is sub section of config.
type SectionKafka struct {
	Brokers      string   `yaml:"brokers"`
	DefaultTopic string   `yaml:"default_topic"`
	HTTPTopics   []string `yaml:"http_topics"`
}

// SectionEtcd is sub section of etcd.
type SectionEtcd struct {
	Addrs           []string `yaml:"addrs"`
	ConnectTTL      int      `yaml:"connect_ttl"`
	RegistryRoot    string   `yaml:"registry_root"`
	ServiceGroup    string   `yaml:"service_group"`
	ServiceName     string   `yaml:"service_name"`
	ServiceProtocol string   `yaml:"service_protocol"`
	ServiceVersion  string   `yaml:"service_version"`
	NodeID          string   `yaml:"node_id"`
}

// LoadConfYaml provide load yml config.
func LoadConfYaml(confPath string) (ConfYaml, error) {
	var config ConfYaml

	configFile, err := ioutil.ReadFile(confPath)

	if err != nil {
		return config, err
	}

	err = yaml.Unmarshal(configFile, &config)

	if err != nil {
		return config, err
	}

	return config, nil
}
