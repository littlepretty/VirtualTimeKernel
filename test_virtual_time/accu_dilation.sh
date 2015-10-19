#!/bin/bash

function err_dilation {
        dil=$1
        factor=$2
        echo "Test accuracy of dilation = ${dil}"
        ./accu_dilation -t $dil -f $factor -e -d -p -b 1000 > accu_dilation_${dil}.log
}

make accu_dilation
dilations="1 2 4 8 16"
for dil in $dilations
do
        #err_dilation $dil 5000
        ./plot_cdf.py --input "accu_dilation_${dil}.log" --dilation $dil -l1 'No dilation' -l2 "Dilation=${dil}" --topic "accu_dilation_${dil}"
done

