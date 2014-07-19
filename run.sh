#!/bin/bash

xterm -e "python ../python/example_runcontrol.py -a tcp://44000" &
sleep 2
xterm -e "python ../python/example_logcollector.py -r tcp://127.0.0.1:44000" &
sleep 2
xterm -e "python ../python/example_datacollector.py -n name1 -r tcp://127.0.0.1:44000" &
sleep 2
xterm -e "python ../python/example_producer.py -r tcp://127.0.0.1:44000" &

