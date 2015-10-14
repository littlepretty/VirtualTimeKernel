#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

def plot_err_cdf():
    """draw cdf to see the error between without/with freeze"""
    err_data = np.loadtxt('err_freeze.log')
    # num_bins = 100
    # counts, bin_edges = np.histogram(data, bins=num_bins)
    # cdf = np.cumsum(counts)
    # cdf = np.divide(cdf, float(1000))
    # plt.plot(bin_edges[1:], cdf)
    # plt.show()
    sorted_err_data = np.sort(err_data)
    length = len(sorted_err_data)
    cdf = np.arange(length) / float(length)

    xlim_min = min(err_data)
    xlim_max = max(err_data)
    plt.xlim(xlim_min, xlim_max)
    plt.plot(sorted_err_data, cdf)
    #plt.show()
    plt.savefig('err_cdf.eps', format='eps')

def plot_compare_cdf():
    """draw cdf to compare without/with freeze elapsed time"""
    data = np.loadtxt('accu_freeze.log')
    no_freeze_data = []
    freeze_data = []
    for x in data:
        no_freeze_data.append(x[0])
        freeze_data.append(x[1])
    num_bins = 500
    counts1, bin_edges1 = np.histogram(no_freeze_data, bins=num_bins)
    cdf1 = np.cumsum(counts1)
    counts2, bin_edges2 = np.histogram(freeze_data, bins=num_bins)
    cdf2 = np.cumsum(counts2)

    xlim_min = min(min(no_freeze_data), min(freeze_data))
    xlim_max = max(max(no_freeze_data), min(freeze_data))
    plt.xlim(xlim_min, xlim_max)
    plt.plot(bin_edges1[1:], cdf1)
    plt.plot(bin_edges2[1:], cdf2)
    #plt.show()
    plt.savefig('compare_cdf.eps', format='eps')

def main():
    """draw 2 cdf figures"""
    plot_err_cdf()
    plot_compare_cdf()

if __name__ == "__main__":
    main()
