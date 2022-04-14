
# yifan
import time
import sys
import math
import copy
import operator
import random
from pyspark import SparkContext
from collections import defaultdict
from itertools import combinations

# ref has no head line
def RMSE(ref, file2):
    sc = SparkContext.getOrCreate()
    sc.setLogLevel("ERROR")
    rdd = sc.textFile(ref)
    fisrtline = rdd.first()
    #ref = rdd.filter(lambda line: line != fisrtline)
    #print(ref.collect())
    ref = rdd.map(lambda line: (line.split(',')[0]+','+line.split(',')[1], (float)(line.split(',')[2])))

    my = sc.textFile(file2)
    fisrtline = my.first()
    my = my.filter(lambda line: line != fisrtline)
    # user_id, business_id, score
    my = my.map(lambda line: (line.split(',')[0]+','+line.split(',')[1], (float)(line.split(',')[2])))
    def mapVal(x):
        a = (x[1][0]-x[1][1])
        return a**2
    evl = ref.join(my).map(lambda x: ("RMSE",mapVal(x))).groupByKey().map(lambda x: ("RMSE", (sum(x[1]), len(list(x[1]))))) \
        .reduceByKey(lambda a, b: (a[0] + b[0], a[1] + b[1])) \
        .mapValues(lambda value: math.sqrt(float(value[0] / value[1])))
    print(evl.collect())


if __name__ == "__main__":
    RMSE(sys.argv[1], sys.argv[2])

    