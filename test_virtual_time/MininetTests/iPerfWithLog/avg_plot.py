#!/usr/bin/python

import sys
import matplotlib.pyplot as plt
import matplotlib
import numpy as np
from math import sqrt

def plot_all(output_file):
    bsl = []
    vir = []
    for i in range(0, num_files):
        one_run = np.loadtxt("bsl_run_%d.txt" % i)
        bsl.append(one_run)

        one_run = np.loadtxt("vir_run_%d.txt" % i)
        vir.append(one_run)

    bsl_avg = np.average(bsl, 0)
    bsl_std = np.std(bsl, 0)
    bsl_conf_int = bsl_std / sqrt(1.0 * len(bsl_std)) * 2.58
    # bsl_avg = np.insert(bsl_avg, 0, 0)
    # bsl_std = np.insert(bsl_std, 0, 0)


    vir_avg = np.average(vir, 0)
    vir_std = np.std(vir, 0)
    vir_conf_int = vir_std / sqrt(1.0 * len(vir_std)) * 2.58
    # vir_avg = np.insert(vir_avg, 0, 0)
    # vir_std = np.insert(vir_std, 0, 0)

    time_start = 11
    time_end = 31
    bw = 800.0
    x_data = range(time_start, time_end)

    print bsl_avg
    print bsl_std

    print vir_avg
    print vir_std

    print x_data

    plt.figure()
    font = {'size':'15'}
    matplotlib.rc('font', **font)
    plt.plot(x_data, bsl_avg, color='blue', marker='s', markersize=8, linewidth=1.0, label='No Freeze')
    plt.plot(x_data, vir_avg, color='red', marker='v', markersize=8, linewidth=1.0, label='Freeze Duration=1s, Interval=1s')

    plt.errorbar(x_data, bsl_avg, bsl_std, color='blue', ecolor='blue')
    plt.errorbar(x_data, vir_avg, vir_std, color='red', ecolor='red')
    plt.grid(True)
    plt.legend(loc='lower right')
    plt.xlim([time_start, time_end-1])
    plt.xticks(range(time_start+1, time_end+1, 2))
    plt.yticks(range(int(bw * 0.5), int(bw) + 1, 100))
    
    plt.ylim([bw * 0.5, bw])
    plt.xlabel('Time Series (Seconds)', fontsize=20)
    plt.ylabel('iPerf Throughput (Mbps)', fontsize=20)
    plt.savefig(output_file, format='eps')

if __name__ == '__main__':
    num_files = int(sys.argv[1])
    plot_all(sys.argv[2])

