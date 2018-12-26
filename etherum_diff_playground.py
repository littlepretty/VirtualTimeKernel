#!/usr/bin/python3.7

# <codecell>
import numpy as np
import glog as log
from pprint import pprint
from matplotlib import pyplot as plt

font = {'family': 'normal', 'size': 28}
matplotlib.rc('font', **font)
matplotlib.rcParams["figure.figsize"] = [14, 10]


# <codecell>
def genDiffAndBlockInterval(numIters, hashRate, initDifficulty=1048576, tdf=1.0):
    """
    Return D = difficulty sequence,
    T = dilated mining time, emuT = emulation running time.
    """
    C_T = 1
    hashRateFactor = 1.0
    D = [initDifficulty]  #[0x400000000]
    BN = [1]
    T, emuT = [], []
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
        if len(T) > 0:
            newT += noiseFactor * np.random.uniform(min(T), max(T))
        if i != 0 and i % 1000 == 0:
            hashRate *= hashRateFactor

        if 1 - (D[-1] / hashRate) // 10 >= -99:
            if state == 2:
                log.info(f'State transition from 2 to 1 at step {i}')
                state = 1
        else:
            if state == 1:
                log.info(f'State transition from 1 to 2 at step {i}')
                state = 2

        T.append(newT / tdf)
        emuT.append(newT)
        newD = D[-1] + D[-1] // 2048 * max(1 - (D[-1] / hashRate) // 10, -99)
        BN.append(int(2**(i // 1e5 - 2)))
        D.append(newD)

    log.info(f'Min difficulty = {np.min(D)}, Max difficulty = {np.max(D)}')
    log.info(f'HP_I = {implicitHP}')
    log.debug(f'Min D - HP_I = {np.min(D) - implicitHP}')
    log.debug(f'Max D - HP_I = {np.max(D) - implicitHP}')
    return D, T, emuT


def plotConvergenceWithFloor():
    hashRate = 2**26
    numIters=3000

    # <codecell>
    smallInitD, smallT, smallEmuT = genDiffAndBlockInterval(numIters=numIters, hashRate=hashRate, initDifficulty=8 * hashRate)
    mediumInitD, mediumT, mediumEmuT = genDiffAndBlockInterval(numIters=numIters, hashRate=hashRate, initDifficulty=80 * hashRate)
    largeInitD, largeT, largeEmuT = genDiffAndBlockInterval(numIters=numIters, hashRate=hashRate, initDifficulty=1800 * hashRate)
    constInitD, constT, constmuT = genDiffAndBlockInterval(numIters=numIters, hashRate=hashRate, initDifficulty=15 * hashRate)
    plt.figure()
    plt.grid()
    plt.semilogy(range(len(smallInitD)), smallInitD, 'b--')
    plt.semilogy(range(len(mediumInitD)), mediumInitD, 'k-.')
    plt.semilogy(range(len(largeInitD)), largeInitD, 'r:')
    plt.semilogy(range(len(constInitD)), constInitD, 'g-')
    plt.xlabel('Time Step')
    plt.ylabel('Ehtereum Difficulty / Log Scale')
    plt.grid(ls='-.')
    plt.subplots_adjust(left=0.12, bottom=0.12, right=0.96, top=0.97)
    plt.legend()
    plt.savefig('ConvergeEquation8.pdf', fmt='pdf')


# <codecell>
Dvt, Tvt, emuTvt, BNvt = genDiffAndBlockInterval(numIters=20000, tdf=0.5)
print(Tvt[:10])
print(f'avgD = {np.mean(Dvt)}, avgT = {np.mean(Tvt)}')
plt.semilogy(range(len(Dvt)), Dvt)
plt.figure()
plt.plot(range(len(Tvt)), Tvt, 'b--')

# <codecell>
ratioD = np.divide(Dvt, D)
ratioT = np.divide(Tvt, T)
ratioEmuT = np.divide(emuTvt, emuT)
print(f'diff ratio = {np.mean(ratioD)}')
print(f'mining time ratio = {np.mean(ratioT)}')
print(f'running time ratio = {np.mean(ratioEmuT)}')


# <codecell>
def splittedAnalysis(numIter=40000, initDifficulty=1048576):
    D1 = [initDifficulty] # [172000000000]
    D2 = [initDifficulty]
    BN = [1]
    hashRate = 2**26
    alpha1 = 1 + 1 / 2**11
    beta1 = -1 / (2048 * 10 * hashRate)
    alpha2 = (1 - 99 / 2048)
    monoThreshold = -1 * alpha1 / (2 * beta1)
    limitD = (1 - alpha1) / beta1
    log.info(f'Monotonic threshold is D <= {monoThreshold}')
    log.info(f'Monotonic limit of D = {limitD}')
    log.info(f'Initial D = {D1[-1]}')
    noises = np.random.exponential(1 / 100, numIter)
    noises = 1 - (noises - np.min(noises)) / (np.max(noises) - np.min(noises))
    for i in range(numIter):
        BN.append(int(2**(i // 1e5 - 2)))
        newD1 = D1[-1] * alpha1 + D1[-1] * D1[-1] * beta1
        D1.append(newD1)
        newD2 = alpha2 * D2[-1]
        D2.append(newD2)

    return (D1, D2)
