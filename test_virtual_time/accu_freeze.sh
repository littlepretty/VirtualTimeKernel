#!/bin/bash

OUTPUT=accu_freeze.log
NUM_TESTS=1000

function unit_test {
        ./time_long_loop -d 20
        setsid ./time_long_loop -v -d 20 &
        pid=$!
        sudo ./freeze_other -w 100 -f 8000 -p $pid
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


