#!/bin/bash

SWs="2"
PingCount=1000
Latencies="10.0 1.0 0.1 0.01 0.001" # microseconds

Durations="0.1" # seconds
Intervals="0.5" # seconds

for n in $SWs 
do
        for delay in $Latencies 
        do
                for dur in $Durations 
                do
                        for int in $Intervals 
                        do
                                echo "***********************************************************"
                                echo "******* Freeze ${dur}s for every ${int}s with ${delay}ms link ******"
                                echo "***********************************************************"
                                ./mn_ping.py -n $n --ping_count $PingCount --latency $delay --duration $dur --interval $int
                                
                                ./parse_ping.pl Sw${n}Lat${delay}Frz${dur}Int${int}Bsl.log > Sw${n}Lat${delay}Frz${dur}Int${int}Bsl.ping
                                ./parse_ping.pl Sw${n}Lat${delay}Frz${dur}Int${int}Vir.log > Sw${n}Lat${delay}Frz${dur}Int${int}Vir.ping
                                
                                ./plot_cdf.py --bsl_file Sw${n}Lat${delay}Frz${dur}Int${int}Bsl.ping --vir_file Sw${n}Lat${delay}Frz${dur}Int${int}Vir.ping --label1 "No Freeze: Link Delay=${delay}us" --label2 "Freeze: Duration=${dur}s  Interval=${int}s" --topic_name Sw${n}Lat${delay}Frz${dur}Int${int}
                        done
                done
        done
done


