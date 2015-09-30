#!/bin/bash

make accu_gtod
OUTPUT=accuracy.log
sudo ./test_accuracy -t 2 -d -e -p > accuracy.log
sudo ./test_accuracy -t 4 -d -e -p >> accuracy.log
sudo ./test_accuracy -t 8 -d -e -p >> accuracy.log
sudo ./test_accuracy -t 16 -d -e -p >> accuracy.log
cat accuracy.log
