#!/usr/bin/env python2
from PyEUDAQWrapper import * # load the ctypes wrapper
from time import sleep
import numpy # for data handling

print "Starting PyDataCollector"
# create PyProducer instance
pp = PyDataCollector("testdatacollector","tcp://127.0.0.1:44000","44002","1")


