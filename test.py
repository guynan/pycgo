""" 
Purposely naive prime number attempt so that we can highlight the 
differences in performance between native python code and calling go
from Python through my C wrapper
"""

import goprime

def isPrime(x):
    
    if x < 2:
        return 0;
    i = 2;

    while(i <= x):
        if i == x:
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

testprimes(100000, goprime.isprime)
#testprimes(100000, isPrime)

