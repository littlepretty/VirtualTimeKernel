#!/bin/bash

function err_dilation {
        dil=$1
        factor=$2
        echo "Test accuracy of dilation = ${dil}, duration factor = ${factor}"
        ./accu_dilation -t $dil -f $factor -e -d -p -o "AccuDil${dil}Dur${factor}.log"
        if [ "$dil" -eq "1" ]
        then
                awk "$AwkScriptVariable" AccuDil${dil}Dur${factor}.log
        fi
}

make accu_dilation

read -d '' AwkScriptVariable << 'EOF'
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
                print total1 / lines, total2/lines >> "avg.log";
        }
}
EOF

dilations="1 2 4"
durations="100 200 400 800 1000 10000 40000 80000 100000 400000 800000 1000000 4000000 8000000 10000000" # in microseconds

# durations="20000 18000 16000 14000 12000 10000 8000 6000 4000 2000 1000 500 100"

for dil in $dilations
do
        > avg.log
        for dur in $durations
        do
                err_dilation $dil $dur
        done
        ./plot_avg.py --input "avg.log"
done

# ./plot_cdf.py --input "accu_dilation_${dil}.log" --dilation $dil -l1 'No dilation' -l2 "Dilation=${dil}" --topic "accu_dilation_${dil}"









