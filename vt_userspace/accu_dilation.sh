#!/bin/bash

function err_dilation {
        dil=$1
        dur=$2
        echo "Test accuracy of dilation = ${dil}, duration = ${dur}"
        ./accu_dilation -t $dil -u $dur -e -d -p -o "AccuDil${dil}Dur${dur}.log"

        awk "$AwkScriptVariable" AccuDil${dil}Dur${dur}.log >> AvgDil${dil}.log
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
                print total1/lines, total2/lines;
        }
}
EOF

#dilations="1 10 20 50"
#durations="100 200 400 800 1000 10000 40000 80000 100000 400000 800000 1000000 4000000 8000000 10000000" # in microseconds
dilations="4"
durations="400000"

for dil in $dilations
do
        > AvgDil${dil}.log
        for dur in $durations
        do
                err_dilation $dil $dur
        done
        ./plot_avg.py --dilation $dil --input "AvgDil${dil}.log" --output "ErrDil${dil}.eps"
done

# ./plot_cdf.py --input "accu_dilation_${dil}.log" --dilation $dil -l1 'No dilation' -l2 "Dilation=${dil}" --topic "accu_dilation_${dil}"
