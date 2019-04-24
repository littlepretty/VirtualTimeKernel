#!/bin/bash

OUTFILE=trace.log
CMD=date

echo "*******************************************"
echo "*********** Start recording ***************"
mount -t debugfs none /sys/kernel/debug
trace-cmd record -e syscalls -p function_graph -o trace.dat $CMD

echo "*******************************************"
echo "******* Filter output to trace.log ********"
trace-cmd report -F '.*:COMM == "'${CMD}'"' > $OUTFILE

echo "*******************************************"
echo "******* Grep time-related functions *******"
grep --color=always -A2 -B16 "ktime\|gettime\|getnstime" $OUTFILE

echo "**************** Done *********************"
