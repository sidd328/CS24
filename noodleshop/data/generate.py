#! /usr/bin/env python

import math

class ArrivalRate(object):
    def __init__(self, ambient, center, scale=1.0, power=1000):
        self.ambient = ambient
        self.center  = center
        self.scale   = scale
        self.power   = power

    def eval(self, minute):
        x = (minute / 720.0 - self.center) * math.pi
        return self.scale * math.pow(math.cos(x), self.power) + self.ambient


class Noodle(object):
    def __init__(self, name, batch_size, cook_time, ingredient_cost, serving_price, arrival_rate):
        self.name            = name
        self.batch_size      = batch_size
        self.cook_time       = cook_time
        self.ingredient_cost = ingredient_cost
        self.serving_price   = serving_price
        self.arrival_rate    = arrival_rate

    def customers(self):
        total = 0.0
        for minute in range(720):
            total += self.rate(minute)
        return total

    def profit(self):
        customers = self.customers()
        income    = customers * self.serving_price
        cogs      = customers * self.ingredient_cost / self.batch_size
        return income - cogs

    def rate(self, minute):
        return self.arrival_rate.eval(minute)

    def to_tsv(self):
        return '\t'.join(map(str, [
            self.batch_size,
            self.cook_time,
            self.ingredient_cost,
            self.serving_price,
            self.name
        ]))

npots  = 5

noodles = [ # Simple
    Noodle('ravioli',    20, 10, 1200, 325, ArrivalRate(0.2, 0, 0)),
    Noodle('spaghetti',  20,  7, 1100, 300, ArrivalRate(0.2, 0, 0)),
    Noodle('tortellini', 20, 12, 1500, 395, ArrivalRate(0.2, 0, 0))
]

# noodles = [ # Chef's Special
#     Noodle('specialty noodles',   20, 10, 1700, 285, ArrivalRate(0.50, 0, 0)),
#     Noodle('generic noodles',     20, 10, 1150, 250, ArrivalRate(0.10, 0, 0)),
#     Noodle('mediocre noodles',    20, 10, 1400, 305, ArrivalRate(0.08, 0, 0)),
#     Noodle('boring noodles',      20, 10, 1200, 275, ArrivalRate(0.06, 0, 0)),
#     Noodle('uninspiring noodles', 20, 10, 1000, 215, ArrivalRate(0.04, 0, 0))
# ]

# npots   = 20
# noodles = [ # Custom Noodles
#     Noodle('Champon',             1, 6, 850, 1100, ArrivalRate(0.030, 0, 0)),
#     Noodle('Hakata ramen',        1, 6, 900, 1200, ArrivalRate(0.030, 0, 0)),
#     Noodle('Hokkaido ramen',      1, 6, 875, 1150, ArrivalRate(0.030, 0, 0)),
#     Noodle('Hiyashi chuka',       1, 6, 710, 1000, ArrivalRate(0.030, 0, 0)),
#     Noodle('Kagoshima ramen',     1, 6, 995, 1300, ArrivalRate(0.030, 0, 0)),
#     Noodle('Muroran curry ramen', 1, 6, 850, 1150, ArrivalRate(0.030, 0, 0)),
#     Noodle('Sanratanmen',         1, 6, 895, 1250, ArrivalRate(0.030, 0, 0)),
#     Noodle('Tonkotsu ramen',      1, 6, 775, 1200, ArrivalRate(0.030, 0, 0)),
#     Noodle('Tsukemen',            1, 6, 800, 1225, ArrivalRate(0.030, 0, 0))
# ]

# noodles = [ # Lunch Rush
#     Noodle('casunziei', 20, 10, 1000, 310, ArrivalRate(0.1, 0.5, 1,   20)),
#     Noodle('mezzelune', 20, 10, 1250, 350, ArrivalRate(0.1, 0.5, 2,  200)),
#     Noodle('ravioli',   20, 10, 1500, 395, ArrivalRate(0.1, 0.5, 4, 2000))
# ]

# noodles = [ # Breakfast, Lunch, and Dinner
#     Noodle('breakfast noodles', 20, 10, 1250, 325, ArrivalRate(0.1, 0.2, 1, 20)),
#     Noodle('lunch noodles',     20, 10, 1000, 300, ArrivalRate(0.1, 0.5, 1, 20)),
#     Noodle('dinner noodles',    20, 10, 1500, 395, ArrivalRate(0.1, 0.8, 1, 20))
# ]

rent   = sum(noodle.profit()    for noodle in noodles) * 0.75
expect = sum(noodle.customers() for noodle in noodles)

# Metadata Line
print('\t'.join(map(lambda x: str(round(x)), [npots, rent, expect, len(noodles)])))

# Noodle Information
for noodle in noodles:
    print(noodle.to_tsv())

# Order Arrival Rates
for minute in range(720):
    # print(minute, end='\t')
    print('\t'.join('%.3f' % noodle.rate(minute) for noodle in noodles))

# Extra Cooking Time
for minute in range(30):
    print('\t'.join(['0'] * len(noodles)))
