from random import randint
from math import pow
n_max = pow(2, 28)
with open('testlargo.txt', 'w') as file:
    for i in range(0, 5000):
        n = str(randint(0, n_max))
        file.writelines([n, "\n"])

