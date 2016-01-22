#!/bin/bash

SWs="1"
PerfTime=30
bw="800" # mb
REPEAT=64

Durations="1.0" # seconds
Intervals="1.0" # 0.5 0.1 0.05" # seconds

dur=1.0
int=1.0

#ovsdb=`pgrep ovsdb-server`
#vswitchd=`pgrep ovs-vswitchd`
#echo "echo 1000 > /proc/${ovsdb}/dilation"
#echo "echo 1000 > /proc/${vswitchd}/dilation"
#echo 1000 > /proc/${ovsdb}/dilation
#echo 1000 > /proc/${vswitchd}/dilation

#echo "************************************************************"
#echo "****** Freeze ${dur}s for every ${int}s with ${bw}mb link ******"
#echo "************************************************************"
#for ((i = 0; i < $REPEAT; ++i))
#do
        #> Sw${SWs}Bw${bw}Frz${dur}Int${int}Bsl.log
        #> Sw${SWs}Bw${bw}Frz${dur}Int${int}Vir.log
        #./mn_iperf.py -n $SWs --perf_time $PerfTime --bw $bw \
                #--duration $dur --interval $int
        #./parse_iperf.awk Sw${SWs}Bw${bw}Frz${dur}Int${int}Bsl.log > bsl_run_${i}.txt
        #./parse_iperf.awk Sw${SWs}Bw${bw}Frz${dur}Int${int}Vir.log > vir_run_${i}.txt
#done

python avg_plot.py $REPEAT iPerfRateAvg${bw}M.eps $bw

#for dur in $Durations
#do
        #for int in $Intervals
        #do
                #echo "************************************************************"
                #echo "****** Freeze ${dur}s for every ${int}s with ${bw}mb link ******"
                #echo "************************************************************"
                #./mn_iperf.py -n $SWs --perf_time $PerfTime --bw $bw \
                        #--duration $dur --interval $int
                #cat Sw${SWs}Bw${bw}Frz${dur}Int${int}Bsl.log
                #cat Sw${SWs}Bw${bw}Frz${dur}Int${int}Vir.log
        #done
#done


