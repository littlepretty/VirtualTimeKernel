#!/bin/bash

./run_long_loop &
PID=$!
echo $PID
./issue_freeze -p $PID

