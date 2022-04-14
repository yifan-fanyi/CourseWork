
# yifan
from re import M
import time
import sys
import math
import copy
import operator
import random
from pyspark import SparkContext
from collections import defaultdict
from itertools import combinations

class Task1:
    def genHash(a, b, m):
        def myHash(x):
            return (a*x+b) % m
        return myHash
    
    def genHashs(n_funcs, baskets):
        a = random.sample(range(1, sys.maxsize-1), n_funcs)
        b = random.sample(range(1, sys.maxsize-1), n_funcs)
        funcs = []
        for i in range(n_funcs):
            funcs.append(Task1.genHash(a[i], b[i], baskets))
        return funcs

    def Jaccard(a, b):
        x = float(len(set(a) | set(b)))
        y = float(float(len(set(a) & set(b))))
        if x == 0:
            return 0
        else:
            return y/x
    
    def myMin(list1, list2):
        m = []
        for i in range(len(list1)):
            m.append(min(list1[i],list2[i]))
        return m

    def split(data, chunk_num):
        res = []
        size = int(math.ceil(len(data) / int(chunk_num)))
        for i, s in enumerate(range(0, len(data), size)):
            res.append((i, hash(tuple(data[s:s + size]))))
        return res

    def getSimilar(cands, data, mp,  TH):
        imp = {v: k for k, v in mp.items()}
        res, tmp = [],  set()
        for pair in cands:
            if pair not in tmp:
                tmp.add(pair)
                s = Task1.Jaccard(data.get(pair[0], set()),
                                  data.get(pair[1], set()))
                if s >= TH:
                    p = [imp[pair[0]],imp[pair[1]]]
                    p.sort()
                    res.append(p[0]+','+p[1]+','+str(s))
        res.sort()
        return res
    
    def toFile(file, data):
        with open(file, 'w+') as f:
            f.write('business_id_1,business_id_2,similarity\n')
            for line in data:
                f.write(line + "\n")
    
    def run(sc, trainfile, outfile):
        n_hash_funcs = 50
        n_bands = 30
        TH = 0.5

        rdd = sc.textFile(trainfile)
        fisrtline = rdd.first()
        data = rdd.filter(lambda line: line != fisrtline)
        data = data.map(lambda line: (line.split(',')[0], line.split(',')[1]))
        # user_id: idx
        user = data.map(lambda x: x[0]).distinct() \
                .sortBy(lambda item: item) \
                .zipWithIndex() \
                .map(lambda x: {x[0]: x[1]}) \
                .flatMap(lambda x: x.items())
        user_dict = user.collectAsMap()

        # business_id: idx
        business = data.map(lambda x: x[1]).distinct() \
                .sortBy(lambda item: item) \
                .zipWithIndex() \
                .map(lambda x: {x[0]: x[1]}) \
                .flatMap(lambda x: x.items())
        business_dict = business.collectAsMap()

        hash_funcs = Task1.genHashs(n_funcs=n_hash_funcs, baskets=len(user_dict)*2)
        user_hashed = user \
                .map(lambda x: (user_dict[x[0]], [func(x[1]) for func in hash_funcs]))
        
        # user_id: [business_id, business_id,...]
        user_business = data \
            .map(lambda x: (user_dict[x[0]], business_dict[x[1]])) \
            .groupByKey() \
            .map(lambda x: (x[0], list(set(x[1]))))
        # business_id: [user_id, user_id,...]
        business_user_dict = data \
            .map(lambda x: (business_dict[x[1]], user_dict[x[0]])) \
            .groupByKey() \
            .map(lambda x: {x[0]: list(set(x[1]))}) \
            .flatMap(lambda x: x.items()).collectAsMap()

        signature = user_business.leftOuterJoin(user_hashed) \
            .map(lambda x: x[1]) \
            .flatMap(lambda x: [(xx, x[1]) for xx in x[0]]) \
            .reduceByKey(Task1.myMin)

        candidate_pairs = signature \
            .flatMap(lambda x: [(tuple(chunk), x[0]) for chunk in Task1.split(x[1], n_bands)]) \
            .groupByKey().map(lambda x: list(x[1])) \
            .filter(lambda x: len(x) > 1) \
            .flatMap(lambda x: [pair for pair in combinations(x, 2)])

        res = Task1.getSimilar(set(candidate_pairs.collect()),
                                business_user_dict,
                                business_dict,
                                TH=TH)

        Task1.toFile(outfile, res)

if __name__ == "__main__":
    sc = SparkContext.getOrCreate()
    sc.setLogLevel("ERROR")
    Task1.run(sc, sys.argv[1], sys.argv[2])
