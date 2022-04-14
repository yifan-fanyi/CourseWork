
# yifan
import time
import sys
import math
import copy
import operator
import csv
from pyspark import SparkContext
from collections import defaultdict
from itertools import combinations

class Task1:
    def select(baskets, cans, S):        
        if len(cans) == 0:
            return set()
        result = set()
        counter = defaultdict(int)
        for k in cans.keys():
            for basket in baskets:
                if k.issubset(basket):
                    for can in cans[k]:
                        if can.issubset(basket):
                            counter[can] += 1
        for itemset, ct in counter.items():
            if ct >= S:
                result.add(itemset)
        return result

    def Apriori_phase1(baskets, num_basket, S):
        baskets = list(baskets)
        local_threshold = math.ceil((len(list(baskets)) / num_basket) * S)
        can = set()
        counts = defaultdict(int)
        for basket in baskets:
            for item in basket:
                counts[item] +=1
        l = set()
        for itemset, ct in counts.items():
            if ct >= local_threshold:
                l.add(frozenset({itemset}))
        res = dict()
        length = 2
        while len(l) > 0:
            res[length - 1] = l
            l = list(l)
            can = set()
            can = defaultdict(list)
            #print(len(l))
            for i in range(len(l)):
                for j in range(i+1, len(l)):
                    temp = set(l[i]).union(set(l[j]))
                    if len(temp) == length:
                        can[frozenset(l[i])].append(frozenset(temp))
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
            
class Task2:
    def preprocess(infile, outfile, TH):
        sc = SparkContext.getOrCreate()
        rdd = sc.textFile(infile)
        fisrtline = rdd.first()
        data = rdd.filter(lambda line: line != fisrtline)
        data = data.map(lambda line: (
            #line.split(',')[1][1:-1], 
            line.split(',')[0][1:-1]+'-'+ line.split(',')[1][1:-1],
            (line.split(',')[0][1:-1]+'-'+ line.split(',')[1][1:-1], (int)(line.split(',')[5][1:-1])))) \
            .groupByKey().map(lambda rec: (rec[0], tuple(set(rec[1])))) \
            .filter(lambda rec: len(rec[1]) > int(TH)) \
            .map(lambda rec: rec[1]).collect()
        
        with open(outfile, 'w', newline='') as f:
            writer = csv.writer(f, quoting=csv.QUOTE_NONE)
            writer.writerow(["DATE-CUSTOMER_ID", "PRODUCT_ID"])
            for row in data:
                for i in row:
                    writer.writerow(i)
    
    def partitioner(k):
        return ord(k[0])

if __name__ == "__main__":
    TH = (int)(sys.argv[2])
    Task2.preprocess(sys.argv[3], './data/tmp.csv', sys.argv[1])
    t0 = time.time()
    sc = SparkContext.getOrCreate()
    sc.setLogLevel("ERROR")
    rdd = sc.textFile('./data/tmp.csv')
    fisrtline = rdd.first()
    data = rdd.filter(lambda line: line != fisrtline)
    data = data.map(lambda line: (line.split(',')[0], line.split(',')[1]))
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
    Task1.tof(copy.deepcopy(candidate), res, sys.argv[4])
    d = time.time() - t0
    print("Duration: %d sec" % (d))  
   # with open(sys.argv[4], 'a') as f:
   #     f.write('Duration: %f'%(d))
          

    