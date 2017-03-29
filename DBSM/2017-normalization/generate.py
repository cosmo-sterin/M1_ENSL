# Implements the generate feature
# numpy for shuffling
import numpy as np
from FD import *

def generate(n):
    fd_list = []
    for i in range(1,n+1):
        fd_list.append(FD([i-1],[i]))
    np.random.shuffle(fd_list)
    for fd in fd_list:
        print(fd)