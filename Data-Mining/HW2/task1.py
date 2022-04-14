
# yifan
from threading import local
import time
import sys
import math
import copy
import operator
from pyspark import SparkContext
from collections import defaultdict
from itertools import combinations

class Task1:
    def select(baskets, cans, S):        
        if len(cans) == 0:
            return set()
        counts = defaultdict(int)
        for basket in baskets:
            for can in cans:
                if can.issubset(basket):
                    counts[can] += 1
        result = set()
        for itemset, ct in counts.items():
            if ct >= S:
                result.add(itemset)
        return result

    def Apriori_phase1(baskets, num_basket, S):
        t0 = time.time()
        baskets = list(baskets)
        local_threshold = math.ceil((len(list(baskets)) / num_basket) * S)
        can = set()
        for basket in baskets:
            for item in basket:
                can.add(frozenset({item}))
        res = dict()
        l = Task1.select(baskets, can, local_threshold)
        length = 2
        while len(l) > 0:
            res[length - 1] = l
            can = set()
            for i in l:
                for j in l:
                    temp = i.union(j)
                    if len(temp) == length:
                        can.add(temp)
            ccan = copy.deepcopy(can) #set cannot be modified during iter
            for item in ccan:
                sbs = combinations(item, length-1)
                for sb in sbs:
                    if (frozenset(sb) not in l):
                        can.remove(item)
                        break
            l = Task1.select(baskets, can, local_threshold)
            length += 1
        result = set()
        for _, items in res.items():
            for item in items:
                result.add(item)
        return result

    def Apriori_phase2(baskets, cans):
        counts = defaultdict(int)
        for basket in list(baskets):
            for can in cans:
                if can.issubset(basket) :
                    counts[can] += 1
        return [(item, count) for item, count in counts.items()]

    def format_write(f, items, iscandidate):
        items = list(map(lambda x: list(x), items))
        for item in items:
            item.sort()
        items.sort(key = lambda x: (len(x), x))
        k, st = 1, 0
        for i in range(len(items)):
            if len(items[i]) > k:
                if k == 1:
                    f.write(','.join("('{}')".format(x[0]) for x in items[st : i]) + '\n\n')
                else:
                    f.write(','.join(str(tuple(x)) for x in items[st : i]) + '\n\n')
                st = i
                k += 1
            i += 1
        if k == 1:
            f.write(','.join("('{}')".format(x[0]) for x in items[st:]) + '\n\n')
        else :
            f.write(','.join(str(tuple(x)) for x in items[st:]) + '\n\n')

    def tof(candidate, freq, path):
        with open(path, 'w') as f:
            f.write('Candidates:\n')
            Task1.format_write(f, candidate, True)
            f.write('Frequent Itemsets:\n')
            Task1.format_write(f, freq, False)

if __name__ == "__main__":
    t0 = time.time()
    case = sys.argv[1]
    TH = (int)(sys.argv[2])
    sc = SparkContext.getOrCreate()
    sc.setLogLevel("ERROR")
    rdd = sc.textFile(sys.argv[3])
    fisrtline = rdd.first()
    data = rdd.filter(lambda line: line != fisrtline)
    if case == "1":
        data = data.map(lambda line: (line.split(',')[0], line.split(',')[1]))
    elif case == "2":
        data = data.map(lambda line: (line.split(',')[1], line.split(',')[0]))
    else:
        assert False, "case error!"
    basket =  data \
        .groupByKey() \
        .mapValues(lambda item: (list)(item)) \
        .map(lambda item:item[1])
    n_basket = basket.count()
    candidate = basket.mapPartitions(
        lambda x: Task1.Apriori_phase1(x, n_basket, TH)).distinct().collect()
    res = basket.mapPartitions(lambda x : Task1.Apriori_phase2(x, candidate)) \
        .reduceByKey(operator.add) \
        .filter(lambda x: x[1] >= TH) \
        .map(lambda x: x[0]).collect()
    Task1.tof(candidate, res, sys.argv[4])
    print("Duration: %d sec" % (time.time() - t0))        