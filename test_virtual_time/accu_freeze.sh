#!/bin/bash

function unit_test {
        dur=$1
        wait_dur=$2
        freeze_dur=$3
        ./time_long_loop -v -d $dur
        setsid ./time_long_loop -v -d $dur &
        pid=$!
        ./freeze_other -w $wait_dur -f $freeze_dur -p $pid
        wait $pid
}

make accu_freeze
read -d '' AwkScript << 'EOF'
BEGIN {
        lines = 0;
        total1 = 0;
        total2 = 0;
}
{
        lines++;
        total1 += $1;
        total2 += $2;
}
END {
        if (lines > 0) {
                print total1/lines, total2/lines;
        }
}
EOF

NUM_TESTS=100
#durations="100 200 400 800 1000" # "2000 4000 6000 8000 10000 20000 40000 60000 80000 100000 200000 400000 600000 800000 1000000"
durations="1000000"

> AvgFrz.log

for dur in $durations 
do 
        > AccuFrz${dur}.log
        echo "Test accuracy of freeze with duration = ${dur}"
        for ((i = 0; i < $NUM_TESTS; ++i)); do
                unit_test $dur 20000 $dur >> AccuFrz${dur}.log
                echo -e "" >> AccuFrz${dur}.log
        done
        awk "$AwkScript" AccuFrz${dur}.log >> AvgFrz.log   
done

./plot_avg.py --dilation 1 --input "AvgFrz.log" --output "ErrFrz.eps"
# ./plot_cdf.py --input $OUTPUT -l1 'Normal' -l2 'Freeze' --topic 'accu_freeze'




