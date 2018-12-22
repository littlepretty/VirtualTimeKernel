#!/usr/bin/python3.7

# <codecell>
import numpy as np
import glog as log
from matplotlib import pyplot as plt

# <codecell>
C_D = 20160
C_T = 2**32
hashRate = 2**32
D = [1.0]
T = []
end = 800
for i in range(end):
    newT = D[-1] / hashRate * C_T
    if len(T) > 0:
        noise = np.random.uniform(min(T) * .1, max(T) * .1)
        newT += noise
    if i % 10 == 0:
        hashRate *= 2
        
    T.append(newT)
    newD = D[-1] / T[-1] * C_D
    D.append(newD)

# <codecell>
plt.plot(range(len(D[0:end // 4])), D[0:end // 4], 'r*')
plt.plot(range(len(D[end // 4: end // 2])), D[end // 4: end // 2], 'r*')
plt.plot(range(len(D[end // 2: end // 4 * 3])), D[end // 2: end // 4 * 3], 'r*')
plt.plot(range(len(D[end // 4 * 3:])), D[end // 4 * 3:], 'r*')

# <codecell>
plt.plot(range(len(T[0:end // 4])), T[0:end // 4], 'b--')
plt.plot(range(len(T[end // 4: end // 2])), T[end // 4: end // 2], 'b--')
plt.plot(range(len(T[end // 2: end // 4 * 3])), T[end // 2: end // 4 * 3], 'b--')
plt.plot(range(len(T[end // 4 * 3:])), T[end // 4 * 3:], 'b--')
