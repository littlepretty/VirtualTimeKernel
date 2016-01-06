#!/bin/bash

SWs="1"
PingCount=100
Latencies="10.0" # microseconds

Durations="0.1" # seconds
Intervals="0.1" # seconds

for n in $SWs 
do
        for delay in $Latencies 
        do
                for dur in $Durations 
                do
                        for int in $Intervals 
                        do
                                echo "***********************************************************"
                                echo "******* Freeze ${dur}s for every ${int}s with ${delay}us link ******"
                                echo "***********************************************************"
                                #./mn_ping.py -n $n --ping_count $PingCount --latency $delay --duration $dur --interval $int
                                #cat Sw${n}Lat${delay}Frz${dur}Int${int}Vir.log  
                                
                                ./parse_ping.pl Sw${n}Lat${delay}Frz${dur}Int${int}Bsl.log > Sw${n}Lat${delay}Frz${dur}Int${int}Bsl.ping
                                ./parse_ping.pl Sw${n}Lat${delay}Frz${dur}Int${int}Vir.log > Sw${n}Lat${delay}Frz${dur}Int${int}Vir.ping
                                
                                ./plot_cdf.py --bsl_file Sw${n}Lat${delay}Frz${dur}Int${int}Bsl.ping --vir_file Sw${n}Lat${delay}Frz${dur}Int${int}Vir.ping --label1 "No Freeze: Delay=${delay}us" --label2 "Freeze: D=${dur}s  I=${int}s" --topic_name Sw${n}Lat${delay}Frz${dur}Int${int}
                        done
                done
        done
done

rm *.ping

