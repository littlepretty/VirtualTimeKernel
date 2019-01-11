#!/usr/bin/python3.7

# <codecell>
import numpy as np
import glog as log
import matplotlib
from pprint import pprint
from matplotlib import pyplot as plt

font = {'family': 'normal', 'size': 28}
matplotlib.rc('font', **font)
matplotlib.rcParams["figure.figsize"] = [14, 10]


def combinedAnalysis(numIters, hashRate, initDifficulty, tdf=1.0):
    D = [initDifficulty]
    BN = [1]
    T, runningT = [0], [0]
    alpha1 = 1 + 1 / 2**11
    beta = -1 / (2048 * 10 * hashRate)
    alpha2 = (1 - 99 / 2048)
    monoThreshold = -1 * alpha1 / (2 * beta)
    caseDividing = 1009 * hashRate
    convergedD = (1 - alpha1) / beta
    log.info(f'Monotonic increasing threshold = {monoThreshold}')
    log.info(f'Converged D = {convergedD}')
    log.info(f'Initial D = {D[-1]}')
    state = 1 if initDifficulty <= caseDividing else 2
    log.info(f'Initial state = {state}')
    for i in range(numIters):
        BN.append(int(2**(i // 1e5 - 2)))
        blockInterval = D[-1] / hashRate
        T.append(T[-1] + blockInterval)
        runningT.append(runningT[-1] + blockInterval * tdf)
        if 1 - D[-1] // (10 * hashRate) >= -99:
            newD = D[-1] * alpha1 + D[-1] * D[-1] * beta
            if state == 2:
                log.info(f'State transition from 2 to 1 at step {i}')
                state = 1
        else:
            newD = D[-1] * alpha2
            if state == 1:
                log.info(f'State transition from 1 to 2 at step {i}')
                state = 2

        D.append(newD)

    log.info(f'Min D - Threshold D = {np.min(D) - monoThreshold}')
    log.info(f'Min D - Converged D = {np.min(D) - convergedD}')
    log.info(f'Max D - Converged D = {np.max(D) - convergedD}')
    return D, T, runningT

def plotConvergeSpeed(numIters=20000, initRatio=8):
    tdf = 0.1
    hashRate = (2**26) * tdf
    D, T, RT = combinedAnalysis(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=initRatio*hashRate,
        tdf=tdf)
    plt.figure()
    plt.loglog(RT, D, 'r--', lw=4.0,
               label='Initial Difficulty $D_0=%dHR^{VT}$, TDF=0.1' % initRatio)
    plt.loglog(T, D, 'g:', lw=4.0,
               label='Initial Difficulty $D_0=%dHR^{VT}$, TDF=0.1' % initRatio)

    hashRate = 2**26
    D, T, RT = combinedAnalysis(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=initRatio*hashRate*tdf,
        tdf=1)
    plt.loglog(RT, D, 'b-.', lw=4.0,
               label='Initial Difficulty $D_0=%dHR^{VT}$, TDF=1.0' % initRatio)

    plt.xlabel('Emulation Running Time (s) / Log Scale')
    plt.ylabel('Ehtereum Difficulty / Log Scale')
    plt.grid(ls='-.')
    plt.subplots_adjust(left=0.12, bottom=0.12, right=0.96, top=0.97)
    plt.legend()
    if initRatio == 8:
        plt.ylim((5e7, 1e9))

    plt.savefig('ConvergeNoFloorSpeedInit%d.pdf' % initRatio, fmt='pdf')
    plt.show()


def plotConvergenceWithoutFloor(tdf=1.0):
    hashRate = (2**26) * tdf
    numIters = 20000
    smallInitD, T, RT = combinedAnalysis(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=8 * hashRate,
        tdf=tdf)
    mediumInitD, T, RT = combinedAnalysis(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=80 * hashRate,
        tdf=tdf)
    largeInitD, T, RT = combinedAnalysis(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=1800 * hashRate,
        tdf=tdf)

    # <codecell>
    plt.figure()
    plt.semilogy(
        range(len(smallInitD)),
        smallInitD,
        'b--',
        label='Initial Difficulty $D_0=8HR$',
        linewidth=4.0)
    plt.semilogy(
        range(len(mediumInitD)),
        mediumInitD,
        'k-.',
        label='Initial Difficulty $D_0=80HR$',
        linewidth=4.0)
    plt.semilogy(
        range(len(largeInitD)),
        largeInitD,
        'r:',
        label='Initial Difficulty $D_0=1800HR$',
        linewidth=4.0)
    plt.xlabel('Time Step')
    plt.ylabel('Ehtereum Difficulty / Log Scale')
    plt.grid(ls='-.')
    plt.subplots_adjust(left=0.12, bottom=0.12, right=0.96, top=0.97)
    plt.legend()
    plt.savefig('ConvergeNoFloorTdf%s.pdf' % tdf, fmt='pdf')
    plt.show()


if __name__ == '__main__':
    plotConvergeSpeed(initRatio=8)
    plotConvergeSpeed(initRatio=160)
    # plotConvergenceWithoutFloor(tdf=0.1)
    # plotConvergenceWithoutFloor(tdf=1.0)
