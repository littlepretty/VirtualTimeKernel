#!/bin/bash

OUTPUT=accu_freeze.log
NUM_TESTS=3

function unit_test {
        ./time_long_loop -d 1000
        setsid ./time_long_loop -v -d 1000 &
        pid=$!
        # ./freeze_other -w 10000 -f 800000 -p $pid
        wait $pid
}

make accu_freeze
# clear content of old output
> $OUTPUT
for ((i = 0; i < $NUM_TESTS; ++i)); do
        unit_test >> $OUTPUT 
        echo -e "" >> $OUTPUT
done

cat $OUTPUT

awk '{ if ($1 > $2) {print $1 - $2;} else { print $2 - $1;}}' $OUTPUT > err_freeze.log
cat err_freeze.log
# ./plot_cdf.py


