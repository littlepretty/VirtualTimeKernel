#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt
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
    num_bins = 100
    counts, bin_edges = np.histogram(variance, bins=num_bins)
    cdf_variance = np.cumsum(counts) / float(len(variance))
    
    xlim_min = min(min(err_data), min(variance)) 
    xlim_max = max(max(err_data), max(variance))
    plt.figure()
    plt.xlim(0.99 * xlim_min, 1.02 * xlim_max)
    
    p1 = plt.plot(sorted_err_data, cdf_err, 'b', label=label_err)
    p2 = plt.plot(bin_edges[1:], cdf_variance, 'r', label='Variance of GTOD')
    
    plt.legend(loc='lower right')
    plt.xlabel('Time / Microseconds')
    plt.ylabel('Cumulative Distribution')
    plt.grid(True)
    #plt.show()
    plt.savefig('err_%s_cdf.eps' % topic_name, format='eps')

def plot_compare_cdf(data1, data2):
    """draw cdf to compare without/with freeze elapsed time"""
    num_bins = 100
    counts1, bin_edges1 = np.histogram(data1, bins=num_bins)
    cdf1 = np.cumsum(counts1) / float(len(data1))
    counts2, bin_edges2 = np.histogram(data2, bins=num_bins)
    cdf2 = np.cumsum(counts2) / float(len(data2))

    xlim_min = min(min(data1), min(data2))
    xlim_max = max(max(data1), max(data2))
    plt.figure()
    plt.xlim(0.99 * xlim_min, 1.02 * xlim_max)
    p1 = plt.plot(bin_edges1[1:], cdf1, 'b', label=label1)
    p2 = plt.plot(bin_edges2[1:], cdf2, 'r', label=label2)
    plt.legend(loc='lower right')
    plt.xlabel('Time Elapsed / Microseconds')
    plt.ylabel('Cumulative Distribution')
    plt.grid(True)
    #plt.show()
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
    plt.xlabel('Absolute Variance / Microseconds')
    plt.ylabel('Cumulative Distribution')
    plt.grid(True)
    #plt.show()
    plt.savefig('var_%s_cdf.eps' % topic_name, format='eps')

def import_data(fname, data1, data2):
    """import 2 column data"""
    data = np.loadtxt(input_file_name)
    for x in data:
        data1.append(int(x[0]))
        data2.append(int(x[1]) * dilation)

def main():
    """draw 2 cdf figures"""
    data1 = []
    data2 = []
    import_data(input_file_name, data1, data2) 
    plot_compare_cdf(data1, data2)
    plot_err_cdf(data1, data2)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', action='store', dest='input_file_name')
    parser.add_argument('--dilation', action='store', dest='dilation', default=1, type=int)
    parser.add_argument('-l1', action='store', dest='label1')
    parser.add_argument('-l2', action='store', dest='label2')
    parser.add_argument('--label_err', action='store', dest='label_err')
    parser.add_argument('--topic', action='store', dest='topic_name')

    results = parser.parse_args()
    input_file_name = results.input_file_name
    dilation = results.dilation
    label1 = results.label1
    label2 = results.label2
    topic_name = results.topic_name
    label_err = 'Abs Error'
    main()

