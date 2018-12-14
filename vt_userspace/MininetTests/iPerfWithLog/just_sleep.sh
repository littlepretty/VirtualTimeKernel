#!/bin/bash

sleep_error() {
        dur=$1
        to_ns=1000000000
        begin=`date +%s%N`
        sleep $dur
        end=`date +%s%N`
        err=$(echo "scale=5;($end-$begin-$dur*$to_ns)/1000" | bc -l)
        echo "sleep $dur seconds with error $err microseconds"
}

self=$$

echo "****** Before entering virtual time ******"
for d in {1..5}
do
        for i in {1..10} 
        do
                sleep_error $d
        done
done

echo "echo 1000 > /proc/$self/dilation"

echo "****** After entering virtual time ******"
for d in {1..5}
do
        for i in {1..10}
        do
                sleep_error $d
        done
done



