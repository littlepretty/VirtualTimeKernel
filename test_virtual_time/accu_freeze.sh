#!/bin/bash

OUTPUT=accu_freeze.log
NUM_TESTS=4000

function unit_test {
        ./time_long_loop -v -d 100
        setsid ./time_long_loop -v -d 100 &
        pid=$!
        ./freeze_other -w 10000 -f 500000 -p $pid
        wait $pid
}

make accu_freeze
# clear content of old output
> $OUTPUT
for ((i = 0; i < $NUM_TESTS; ++i)); do
        sudo unit_test >> $OUTPUT 
        sudo echo -e "" >> $OUTPUT
done

cat $OUTPUT
# awk '{ if ($1 > $2) {print $1 - $2;} else { print $2 - $1;}}' $OUTPUT > err_freeze.log
./plot_cdf.py --input $OUTPUT -l1 'Normal' -l2 'Freeze' --topic 'accu_freeze'

