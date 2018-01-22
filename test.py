""" 
Purposely naive prime number attempt so that we can highlight the 
differences in performance between native python code and calling go
from Python through my C wrapper
"""

class Other:
    
    def __init__(self, k):
        self.k = k;


class This:
    
    def __init__(self, x, cls):
        self.x = x;
        self.cls = cls;



import goprime
import math

def isPrime(x):
    
    if x < 2:
        return 0;
    i = 2;

    ceil = int(math.floor(math.sqrt(float(x)))) + 1

    while(i <= ceil):
        if i == ceil:
            return 1;
        if x % i == 0:
            break;

        i += 1;

    return 0;

def testprimes(n, func):
    res = []
    for i in range(n):
        if func(i):
            res.append(i)

def sanitycheck(func):
    for i in range(20):
        if func(i):
            print(i);

#testprimes(1000000, goprime.isprime)
#testprimes(1000000, isPrime)
n = 10000000
#[x for x in range(n) if goprime.isprime(x)]
print([x for x in range(40) if goprime.isprime(x)])
#goprime.large_init(n);
print(goprime.ret());

#goprime.ret()

