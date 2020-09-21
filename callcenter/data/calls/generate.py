#! /usr/bin/env python

import numpy
import random
import sys

def clamp(x, lo, hi):
    x = round(x)
    x = max(lo, x)
    x = min(hi, x)
    return x


def constant(c):
    def closure(x):
        return c
    return closure

def linear(m, b):
    def closure(x):
        return m * x + b
    return closure

def quadratic(a, b, c):
    def closure(x):
        return a*x*x + b*x + c
    return closure

def normal(m, s):
    def closure(x):
        return numpy.random.normal(m, s)
    return closure

def exponential(m):
    def closure(x):
        return numpy.random.exponential(m)
    return closure

def uniform(lo, hi):
    def closure(x):
        return random.randint(lo, hi)
    return closure

# # Simple
# rate       = linear(0, 0.2)
# importance = uniform(1, 10)
# difficulty = normal(13, 4)
# duration   = normal(25, 5)

# # Random
# rate       = linear(0, 0.3)
# importance = uniform(1, 10)
# difficulty = uniform(1, 25)
# duration   = uniform(1, 45)

# Normal
rate       = linear(0, 0.5)
importance = normal( 5,  3)
difficulty = normal(12,  7)
duration   = normal(25, 15)


for minute in range(540):
    x        = minute / 540.0
    arrivals = numpy.random.poisson(rate(x))
    for _ in range(arrivals):
        imp = clamp(importance(x),   1, 10)
        dif = clamp(difficulty(imp), 1, 25)
        dur = clamp(duration(imp),   1, 45)
        sys.stdout.write('%d\t%d\t%d\t' % (imp, dif, dur))
    print('')

for i in range(60):
    print('')
