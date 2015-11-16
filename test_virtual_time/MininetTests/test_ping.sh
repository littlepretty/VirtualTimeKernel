#!/bin/bash

SWs="3"
PingCount=5
Latencies="0.001"

Durations="1"
Intervals="0.1"

for n in $SWs 
do
        for delay in $Latencies 
        do
                for dur in $Durations 
                do
                        for int in $Intervals 
                        do
                                sudo ./mn_ping.py -n $n --ping_count $PingCount --latency $delay --duration $dur --interval $int

                                output=Sw${sw}Lnk${delay}Frz${dur}Int${int}.log 
                                cat *.log
                        done
                done
        done
done

