#!/bin/bash

rm *.log

SWs="2"
PerfTime=60
BWs="100" # microseconds
Durations="1.0 0.1 0.01" # seconds
Intervals="1.0 0.5 0.1 0.05" # seconds

for bw in $BWs
do
        for dur in $Durations
        do
                for int in $Intervals
                do
                        echo "*******************************************************************"
                        echo "********** Freeze ${dur}s for every ${int}s with ${bw}mb link ***********"
                        echo "*******************************************************************"
                        ./mn_iperf.py -n $SWs --perf_time $PerfTime --bw $bw \
                                --duration $dur --interval $int
                        cat Sw${n}Bw${bw}Frz${dur}Int${int}Bsl.log
                        cat Sw${n}Bw${bw}Frz${dur}Int${int}Vir.log
                done
        done
done

