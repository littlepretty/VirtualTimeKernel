#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import argparse

def plot_err_cdf(data1, data2):
    """draw cdf to see the error between without/with freeze"""
    err_data = []
    for x, y in zip(data1, data2):
        err_data.append(abs(x-y))
    sorted_err_data = np.sort(err_data)
    cdf_err = np.arange(len(sorted_err_data)) / float(len(sorted_err_data))
    
    avg = np.mean(data1)
    variance = []
    for x in data1:
        variance.append(abs(x-avg))
    num_bins = 5000
    counts, bin_edges = np.histogram(variance, bins=num_bins)
    cdf_variance = np.cumsum(counts) / float(len(variance))
    
    xlim_min = min(min(err_data), min(variance)) 
    xlim_max = max(max(err_data), max(variance))
    plt.figure()
    # plt.xlim(0.99 * xlim_min, 1.02 * xlim_max)
    # plt.xlim(0, 5000)
    p1 = plt.plot(sorted_err_data, cdf_err, 'b', label=label_err)
    p2 = plt.plot(bin_edges[1:], cdf_variance, 'r', label='Variance of GTOD')
    
    plt.legend() #(loc='lower right')
    plt.xlabel('Time Error (Milliseconds)', fontsize=20)
    plt.ylabel('Cumulative Distribution', fontsize=20)
    plt.grid(True)
    # plt.show()
    plt.savefig('err_%s_cdf.eps' % topic_name, format='eps')

def plot_compare_cdf(data1, data2):
    """draw cdf to compare without/with freeze elapsed time"""
    num_bins = 5000
    counts1, bin_edges1 = np.histogram(data1, bins=num_bins)
    cdf1 = np.cumsum(counts1) / float(len(data1))
    counts2, bin_edges2 = np.histogram(data2, bins=num_bins)
    cdf2 = np.cumsum(counts2) / float(len(data2))

    xlim_min = min(min(data1), min(data2))
    xlim_max = max(max(data1), max(data2))
    plt.figure()
    # plt.xlim(0.98 * xlim_min, 1.02 * xlim_max)
    # plt.xlim(65000, 70000)
    p1 = plt.plot(bin_edges1[1:], cdf1, 'b', label=label1)
    p2 = plt.plot(bin_edges2[1:], cdf2, 'r', label=label2)
    plt.legend() #(loc='lower right')
    plt.xlabel('PING RTT / Milliseconds', fontsize=20)
    plt.ylabel('Cumulative Distribution', fontsize=20)
    plt.grid(True)
    # plt.show()
    plt.savefig('cmp_%s_cdf.eps' % topic_name, format='eps')

def plot_variance_cdf(data):
    avg = np.mean(data)
    variance = []
    for x in data:
        variance.append(abs(x-avg))
    num_bins = 10
    counts, bin_edges = np.histogram(variance, bins=num_bins)
    cdf = np.cumsum(counts) / float(len(variance))
    
    xlim_min = min(variance)
    xlim_max = max(variance)
    plt.figure()
    plt.xlim(0.99 * xlim_min, 1.02 * xlim_max)
    p = plt.plot(bin_edges[1:], cdf, 'b', label='Variance of GTOD')
    plt.legend(loc='lower right')
    plt.xlabel('Absolute Variance / Microseconds', fontsize=20)
    plt.ylabel('Cumulative Distribution', fontsize=20)
    plt.grid(True)
    #plt.show()
    plt.savefig('var_%s_cdf.eps' % topic_name, format='eps')

def main():
    """draw 2 cdf figures"""
    data1 = np.loadtxt(bsl_file)
    data2 = np.loadtxt(vir_file)
    font = {'size':16}
    matplotlib.rc('lines', lw=2)
    matplotlib.rc('font', **font)
    plot_compare_cdf(data1, data2)
    plot_err_cdf(data1, data2)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-b', '--bsl_file', action='store')
    parser.add_argument('-v', '--vir_file', action='store')
    parser.add_argument('-d', '--dilation', action='store', default=1, type=int)
    parser.add_argument('--label1', action='store')
    parser.add_argument('--label2', action='store')
    parser.add_argument('--label_err', action='store', default='Abs Error')
    parser.add_argument('--topic_name', action='store')

    results = parser.parse_args()
    bsl_file = results.bsl_file
    vir_file = results.vir_file
    dilation = results.dilation
    label1 = results.label1
    label2 = results.label2
    topic_name = results.topic_name
    label_err = results.label_err

    main()

