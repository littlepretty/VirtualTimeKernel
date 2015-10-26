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

dilations="1"
durations="100 200 400 500 800 1000 1500 1800 2000 2500 2800 3000 3200 3600 4000 4400 4800 5000 5200 5800 6000 6400 6800 7200 7500 7800 8000 8200 8600 9000 9200 9600 10000 11000 12000 13000 14000 15000 16000 17000 18000 19000 20000"

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
