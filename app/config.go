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
	Kafka SectionKafka `yaml:"kafka"`
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
	UDPPort         int        `yaml:"udp_port"`
	LogSize         int        `yaml:"log_size"`
}

// SectionKafka is sub section of config.
type SectionKafka struct {
	Brokers string `yaml:"brokers"`
	Topic   string `yaml:"topic"`
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
