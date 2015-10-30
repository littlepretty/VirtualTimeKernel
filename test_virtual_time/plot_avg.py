#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import argparse

def plot_err(data1, data2):
    err_data = []
    for x, y in zip(data1, data2):
        e = abs(x - y * dilation)
        if e < 1:
            e = 1
        err_data.append(e)
    print err_data

    fig = plt.figure()
    
    plt.plot(data1, err_data, '--b^')
    
    plt.xlabel('Duration / Microseconds')
    plt.ylabel('Error / Microseconds')
    plt.grid(True)
    
    ax = fig.add_subplot(1, 1, 1)
    ax.set_xscale('log')
    ax.set_yscale('log')
    # major_ticks = np.arange(0, 1.1 * max(err_data), max(err_data) / 10)
    # ax.set_yticks(major_ticks)
    # plt.ylim(0, 1.1 * max(err_data))
    plt.savefig(output_filename, format='eps')

def import_data(data1, data2):
    data = np.loadtxt(input_filename)
    for x in data:
        data1.append(float(x[0]))
        data2.append(float(x[1]))

def main():
    data1 = []
    data2 = []
    import_data(data1, data2)
    plot_err(data1, data2)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', action='store', dest='input_filename')
    parser.add_argument('--output', action='store', dest='output_filename')
    parser.add_argument('--dilation', action='store', dest='dilation')

    results = parser.parse_args()
    input_filename = results.input_filename
    output_filename = results.output_filename
    dilation = int(results.dilation)
    main()





