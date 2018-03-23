# log-kafka #
---
 * receive udp/http log package and write it to kafka
 
## introdction ##
---
To run the app, compile it firstly:

    $ sh assembly/linux/test.sh

Next, start the app:

    $ cd target/linux/log-kafka-0.0.01-2017*-*-test/ && bash bin/load_log-kafka.sh monitor

If you wanna change sbin name, pls reset the value of TARGET_EXEC_NAME in assembly/common/app.properties.


## LICENCE ##
---
Apache License 2.0

