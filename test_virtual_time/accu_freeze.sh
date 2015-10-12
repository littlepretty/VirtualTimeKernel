#!/bin/bash
OUTPUT=accu_freeze.log
NUM_TESTS=10

function unit_test {
        ./time_long_loop -d 50
        ./time_long_loop -v -d 50 &
        pid=$!
        ./freeze_other -w 500 -f 2 -p $pid
        
        cat /proc/$pid/dilation
        cat /proc/$pid/freeze
        wait $pid
}

make accu_freeze
# clear content of old output
# > $OUTPUT
for ((i = 0; i < $NUM_TESTS; ++i)); do
        unit_test # >> $OUTPUT
        echo -e "" # >> $OUTPUT
done

# cat $OUTPUT
# awk '{ print $1}' $OUTPUT > no_freeze_loop.log
# awk '{ if ($1 > $3) {print $1 - $3;} else { print $3 - $1;}}' $OUTPUT > err_freeze.log
# ./plot_cdf.py


