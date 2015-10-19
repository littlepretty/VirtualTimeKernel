#!/bin/bash

OUTPUT=accu_freeze.log
NUM_TESTS=5000

function unit_test {
        ./time_long_loop -v -d 100
        setsid ./time_long_loop -v -d 100 &
        pid=$!
        ./freeze_other -w 10000 -f 500000 -p $pid
        wait $pid
}

function main {
        # clear content of old output
        > $OUTPUT
        for ((i = 0; i < $NUM_TESTS; ++i)); do
                unit_test >> $OUTPUT 
                echo -e "" >> $OUTPUT
        done
}

make accu_freeze
# main
./plot_cdf.py --input $OUTPUT -l1 'Normal' -l2 'Freeze' --topic 'accu_freeze'

