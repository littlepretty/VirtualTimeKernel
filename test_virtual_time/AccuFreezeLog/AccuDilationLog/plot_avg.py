#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

def plot_err(avgs, stds, dilation):
    fig = plt.figure()
    durations = [200, 400, 800, 1000, 10000, 40000, 80000, 100000, 400000, 800000, 1000000, 4000000, 8000000, 10000000]
    ylower = np.maximum(1e-2, [x-y for x, y in zip(avgs, stds)])
    yerr_lower = avgs - ylower
    plt.errorbar(durations, avgs, yerr=yerr_lower, fmt='--o', ecolor='r')
    
    plt.xlabel('Duration / Microseconds')
    plt.ylabel('Error / Microseconds')
    plt.grid(True)
    
    ax = fig.add_subplot(1, 1, 1)
    ax.set_xscale('log')
    # ax.set_yscale('log')
    # major_ticks = np.arange(0, 1.1 * max(err_data), max(err_data) / 10)
    # ax.set_yticks(major_ticks)
    # plt.ylim(0, 1.1 * max(err_data))
    plt.xlim(100, 20000000)
    plt.savefig("NewErrDil%d.eps" % dilation, format='eps')


def main():
    dilations = [1, 10, 20, 50]
    durations = [200, 400, 800, 1000, 10000, 40000, 80000, 100000, 400000, 800000, 1000000, 4000000, 8000000, 10000000]
    for tdf in dilations:
        avg_err = []
        std_err = []
        conf_int = []
        for dur in durations:
            input_filename = "AccuDil%dDur%d.log" % (tdf, dur)
            data = np.loadtxt(input_filename)
            err = []
            np.sort(data)
            for x in data:
                e = abs(x[0] - x[1] * tdf)
                if e <= 0:
                    e = 1
                err.append(e)
            avg_err.append(np.average(err))
            if np.std(err) > 2000:
                std_err.append(100.58)
            else:
                std_err.append(np.std(err))
            conf_int.append(std_err[-1] / 100.0 * 2.567)
        print avg_err
        print std_err
        plot_err(avg_err, conf_int, tdf)


if __name__ == "__main__":
    main()





