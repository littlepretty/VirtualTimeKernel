#!/bin/bash

./time_long_loop 

./time_long_loop -v &
pid1=$!
./time_long_loop -v &
pid2=$!
./time_long_loop -v &
pid3=$!

# ./freeze_other -w 5 -f 50 -p $pid1 $pid2 $pid3

echo $pid1 $pid2 $pid3  
sleep 1
./freeze_all_procs -f -p $pid1 $pid2 $pid3
#./freeze_all_procs -u -p $pid1 $pid2 $pid3

