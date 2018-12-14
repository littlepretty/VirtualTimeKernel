#!/bin/bash

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

NUM_TESTS=5
#durations="100 200 400 800 1000" # "2000 4000 6000 8000 10000 20000 40000 60000 80000 100000 200000 400000 600000 800000 1000000"
interval=500
durations="100000"

> AvgFrz.log

for dur in $durations
do
        > AccuFrz${dur}.log
        echo "*** Test accuracy of freeze with duration = ${dur}"
        ./freeze_other -f $dur & #>> AccuFrz${dur}.log
        daemon=$!
        echo "*** Start freeze daemon(${daemon})"
        setsid ./freeze_me -s $daemon -i $interval -r $NUM_TESTS #>> AccuFrz${dur}.log

        kill -s SIGINT $daemon
        cat AccuFrz${dur}.log

        # awk "$AwkScript" AccuFrz${dur}.log >> AvgFrz.log
done

# ./plot_avg.py --dilation 1 --input "AvgFrz.log" --output "ErrFrz.eps"
# ./plot_cdf.py --input $OUTPUT -l1 'Normal' -l2 'Freeze' --topic 'accu_freeze'

rm freeze_other freeze_me
