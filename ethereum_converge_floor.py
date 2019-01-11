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


def iterativeDifficulty(numIters, hashRate, initDifficulty=1048576, tdf=1.0):
    """
    Return D = difficulty sequence,
    T = dilated mining time, emuT = emulation running time.
    """
    C_T = 1
    hashRateFactor = 1.0
    D = [initDifficulty]  #[0x400000000]
    BN = [1]
    T, runningT = [0], [0]
    noiseFactor = 0.1
    explicitHP = 1009 * hashRate
    implicitHP = 10 * hashRate
    log.debug(f'Initial D = {D[-1]}')
    log.debug(f'HP_E = {explicitHP}')
    log.debug(f'Initial D - HP_E = {D[-1] - explicitHP}')
    state = 1 if initDifficulty <= explicitHP else 2
    log.info(f'Initial state = {state}')
    for i in range(numIters):
        newT = D[-1] / hashRate * C_T
        # if len(T) > 0:
        #     newT += noiseFactor * np.random.uniform(min(T), max(T))
        # if i != 0 and i % 1000 == 0:
        #     hashRate *= hashRateFactor

        if 1 - (D[-1] / hashRate) // 10 >= -99:
            if state == 2:
                log.info(f'State transition from 2 to 1 at step {i}')
                state = 1
        else:
            if state == 1:
                log.info(f'State transition from 1 to 2 at step {i}')
                state = 2

        T.append(T[-1] + newT)
        runningT.append(runningT[-1] + newT * tdf)
        newD = D[-1] + D[-1] // 2048 * max(1 - (D[-1] / hashRate) // 10, -99)
        BN.append(int(2**(i // 1e5 - 2)))
        D.append(newD)

    log.info(f'Min difficulty = {np.min(D)}, Max difficulty = {np.max(D)}')
    log.info(f'HP_I = {implicitHP}')
    log.debug(f'Min D - HP_I = {np.min(D) - implicitHP}')
    log.debug(f'Max D - HP_I = {np.max(D) - implicitHP}')
    return D, T, runningT


def plotConvergeSpeed(numIters=10000, initRatio=8):
    tdf = 0.1
    hashRate = (2**26) * tdf
    D, T, RT = iterativeDifficulty(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=initRatio*hashRate,
        tdf=tdf)
    plt.figure()
    plt.loglog(RT, D, 'r--', lw=4.0,
               label='TDF=0.1 w.r.t. Wall Clock Time')
    plt.loglog(T, D, 'g:', lw=4.0,
               label='TDF=0.1 w.r.t. Virtual Time')

    hashRate = 2**26
    D, T, RT = iterativeDifficulty(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=initRatio*hashRate*tdf,
        tdf=1)
    plt.loglog(RT, D, 'b-.', lw=4.0,
               label='TDF=1.0 w.r.t. Wall Clock/Virtual Time')

    plt.xlabel('Emulation Running Time (s) / Log Scale')
    plt.ylabel('Ehtereum Difficulty / Log Scale')
    plt.grid(ls='-.')
    plt.subplots_adjust(left=0.12, bottom=0.12, right=0.96, top=0.97)
    plt.legend()
    if initRatio == 8:
        plt.ylim((5e7, 1e9))
    elif initRatio == 400:
        plt.ylim((8e7, 2e10))

    plt.savefig('ConvergeFloorSpeedInit%d.pdf' % initRatio, fmt='pdf')
    plt.show()


def plotConvergenceWithFloor(tdf=1.0):
    hashRate = (2**26) * tdf
    numIters=3000
    smallInitD, smallT, smallEmuT = iterativeDifficulty(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=8*hashRate,
        tdf=tdf)
    mediumInitD, mediumT, mediumEmuT = iterativeDifficulty(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=80*hashRate,
        tdf=tdf)
    largeInitD, largeT, largeEmuT = iterativeDifficulty(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=1800*hashRate,
        tdf=tdf)
    constInitD, constT, constmuT = iterativeDifficulty(
        numIters=numIters,
        hashRate=hashRate,
        initDifficulty=15*hashRate,
        tdf=tdf)
    plt.figure()
    plt.grid()
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
    plt.semilogy(
        range(len(constInitD)),
        constInitD,
        'g-',
        label='Initial Difficulty $D_0=15HR$',
        linewidth=4.0)
    plt.xlabel('Time Step')
    plt.ylabel('Ehtereum Difficulty / Log Scale')
    plt.grid(ls='-.')
    plt.subplots_adjust(left=0.12, bottom=0.12, right=0.96, top=0.97)
    plt.legend()
    plt.savefig('ConvergeFloorTdf%s.pdf' % tdf, fmt='pdf')


if __name__ == '__main__':
    plotConvergeSpeed(initRatio=8)
    plotConvergeSpeed(initRatio=400)
    # plotConvergenceWithFloor(tdf=0.1)
    # plotConvergenceWithFloor(tdf=1.0)
