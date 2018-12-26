#!/usr/bin/python3.7

# <codecell>
import numpy as np
import glog as log
from pprint import pprint
from matplotlib import pyplot as plt

# <codecell>
def genDiffAndBlockInterval1(numIters=1000, tdf=1.0):
    C_D = 20160
    C_T = 2**32
    hashRate = 7 * (2**20)
    hashRateFactor = 1.0
    D = [1.0]
    T = []
    noiseFactor = 0.0
    for i in range(numIters):
        newT = D[-1] / hashRate * C_T
        if len(T) > 0:
            newT += noiseFactor * np.random.uniform(min(T), max(T))
        if i != 0 and i % 100 == 0:
            hashRate *= hashRateFactor
        T.append(newT / tdf)
        newD = D[-1] / T[-1] * C_D
        D.append(newD)

    return (D, T)

# <codecell>
D, T = genDiffAndBlockInterval1()
pprint(f'{D[:32]}')
pprint(f'{T[:32]}')
log.info(f'avgD = {np.mean(D[:-200])}, avgT = {np.mean(T[:-200])}')

# <codecell>
D, T = genDiffAndBlockInterval1(tdf=0.5)
pprint(f'{D[:32]}')
pprint(f'{T[:32]}')
log.info(f'avgD = {np.mean(D[:-200])}, avgT = {np.mean(T[:-200])}')

# <codecell>
plt.plot(range(len(D)), D, 'r--')
plt.figure()
plt.plot(range(len(T)), T, 'b--')

# <codecell>
def genDiffAndBlockInterval2(numIters=840):
    C_D = (2**16 - 1.0) * (2**208)
    C_T = 2**32
    hashRate = 7.0 * (2**20)
    D = [1.0]
    T = []
    for i in range(numIters):
        newT = D[-1] / hashRate * C_T
        if len(T) > 0:
            newT *= np.random.uniform(0, 1)
        if i != 0 and i % 100 == 0:
            hashRate *= 2
        T.append(newT)
        newD = D[-1] / T[-1] * C_D
        D.append(newD)

    return (D, T)


# <codecell>
D, T = genDiffAndBlockInterval2()
log.info(f'{D[:100]}')
plt.plot(range(len(D)), D, 'r-')
# plt.plot(range(len(D[0:end // 4])), D[0:end // 4], 'r--')
# plt.plot(range(len(D[end // 4: end // 2])), D[end // 4: end // 2], 'r*')
# plt.plot(range(len(D[end // 2: end // 4 * 3])), D[end // 2: end // 4 * 3], 'r*')
# plt.plot(range(len(D[end // 4 * 3:])), D[end // 4 * 3:], 'r*')
plt.figure()
plt.plot(range(len(T)), T, 'b--')
# plt.plot(range(len(T[0:end // 4])), T[0:end // 4], 'b--')
# plt.plot(range(len(T[end // 4: end // 2])), T[end // 4: end // 2], 'b--')
# plt.plot(range(len(T[end // 2: end // 4 * 3])), T[end // 2: end // 4 * 3], 'b--')
# plt.plot(range(len(T[end // 4 * 3:])), T[end // 4 * 3:], 'b--')
