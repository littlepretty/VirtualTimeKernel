#!/bin/bash
OUTPUT=accu_freeze.log
NUM_TESTS=1000

function unit_test {
        ./time_long_loop
        ./time_long_loop -v &
        pid=$!
        ./freeze_other -w 10 -f 50 -p $pid
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

