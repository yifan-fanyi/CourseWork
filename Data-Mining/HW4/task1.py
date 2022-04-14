# yifan
import time
import sys
import math
from operator import add
import random
import itertools
from graphframes import GraphFrame
from pyspark.sql import SparkSession
from pyspark import SparkContext
from collections import defaultdict
from itertools import combinations

def Time(method):
    def timed(*args, **kw):
        ts = time.time()
        result = method(*args, **kw)
        te = time.time()
        print("Duration: %4.2f sec"%((te - ts)))
        return result
    return timed

class Task1:
    def toFile(file, data):
        with open(file, 'w+') as f:
            for line in data:
                f.write(str(line)[1:-1] + "\n")

    @Time
    def run(sc, TH, infile, outfile):
        rdd = sc.textFile(infile)
        fisrtline = rdd.first()
        data = rdd.filter(lambda line: line != fisrtline)

        uid = data.map(lambda line: (line.split(',')[0], line.split(',')[1])) \
            .groupByKey() \
            .mapValues(lambda x: sorted(list(x))) \
            .collectAsMap()
         
        cand_pairs = list(itertools.combinations(list(uid.keys()), 2))
        
        edge, vertex = list(), set()
        for pair in cand_pairs:
            if len(set(uid[pair[0]]).intersection(set(uid[pair[1]]))) >= TH:
                edge.append(tuple((pair[0], pair[1])))
                edge.append(tuple((pair[1], pair[0])))
                vertex.add(pair[0])
                vertex.add(pair[1])
        graph = GraphFrame(sc.parallelize(list(vertex)).map(lambda uid: (uid,)).toDF(['id']),
                                 sc.parallelize(edge).toDF(["src", "dst"]))
        communities = graph.labelPropagation(maxIter=5)
        communities = communities.rdd.coalesce(1) \
            .map(lambda idx_label: (idx_label[1], idx_label[0])) \
            .groupByKey() \
            .map(lambda label_idxes: sorted(list(label_idxes[1]))) \
            .sortBy(lambda idxes: (len(idxes), idxes)) \
            .collect()
        Task1.toFile(outfile, communities)

if __name__ == "__main__":
    sc = SparkContext.getOrCreate()
    spark = SparkSession(sc)
    sc.setLogLevel("ERROR")
    Task1.run(sc, (float)(sys.argv[1]), sys.argv[2], sys.argv[3])
