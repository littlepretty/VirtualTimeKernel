#!/bin/bash

rm *.log

SWs="2"
PingCount=200
Latencies="5.0 1.0 0.1 0.01 0.001" # microseconds

Durations="0.1 0.01" # seconds
Intervals="0.5 0.1 0.05" # seconds

for n in $SWs 
do
        for delay in $Latencies 
        do
                for dur in $Durations 
                do
                        for int in $Intervals 
                        do
                                echo "***********************************************************"
                                echo "****** Freeze ${dur}s for every ${int}s with ${delay}ms link ******"
                                echo "***********************************************************"
                                ./mn_ping.py -n $n --ping_count $PingCount \
                                        --latency $delay --duration $dur --interval $int
                                cat Sw${n}Lat${delay}Frz${dur}Int${int}Bsl.log 
                                cat Sw${n}Lat${delay}Frz${dur}Int${int}Vir.log
                        done
                done
        done
done

