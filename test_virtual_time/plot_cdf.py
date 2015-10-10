#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('err_freeze.log')
no_freeze_data = np.loadtxt('no_freeze_loop.log')
std_loop = np.nanstd(no_freeze_data)
print 'stdev of loops are %d' % std_loop

# num_bins = 100
# counts, bin_edges = np.histogram(data, bins=num_bins)
# cdf = np.cumsum(counts)
# cdf = np.divide(cdf, float(1000))
# plt.plot(bin_edges[1:], cdf)
# plt.show()

sorted_data = np.sort(data)
sorted_data -= std_loop

length = len(sorted_data)
yvals = np.arange(length) / float(length)
avg_data = np.average(sorted_data)
print 'average of errors are %d' % avg_data
np.savetxt('sort_err_freeze.log', sorted_data, fmt='%d')
plt.plot(sorted_data, yvals)
plt.show()

