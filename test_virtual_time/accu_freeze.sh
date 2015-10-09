#!/bin/bash
OUTPUT=accu_freeze.log
NUM_TESTS=10000

function unit_test {
        ./time_long_loop
        ./time_long_loop -v -d 20&
        pid=$!
        ./freeze_other -w 5 -f 10 -p $pid
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

awk '{ print $1}' $OUTPUT > no_freeze_loop.log
awk '{ if ($1 > $3) {print $1 - $3;} else { print $3 - $1;}}' $OUTPUT > err_freeze.log
./plot_cdf.py


