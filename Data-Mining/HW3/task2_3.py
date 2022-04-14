import time
import sys
import os
from pyspark import SparkContext, SparkConf
from task2_1 import Task2_1
from task2_2 import Task2_2

class Task2_3:
    def mapVal(x):
        a, b, c = x[0], x[1][0], x[1][1]
        if x[1][1] is None:
            return (a, b)
        return (a, 0.1*b+0.9*c)

    def run(sc, folder, testfile, outfile):
        t0 = time.time()
        sc = SparkContext.getOrCreate()
        sc.setLogLevel("ERROR")
        Task2_1.run(sc, os.path.join(folder, 'yelp_train.csv'), testfile, 'tmp_task2_1.txt')
        Task2_2.run(sc, folder, testfile, 'tmp_task2_2.txt')
        rdd = sc.textFile('tmp_task2_1.txt')
        fisrtline = rdd.first()
        p1 = rdd.filter(lambda line: line != fisrtline).map(lambda line: (line.split(',')[0]+','+line.split(',')[1], (float)(line.split(',')[2])))
        rdd = sc.textFile('tmp_task2_2.txt')
        fisrtline = rdd.first()
        p2 = rdd.filter(lambda line: line != fisrtline).map(lambda line: (line.split(',')[0]+','+line.split(',')[1], (float)(line.split(',')[2])))
        p = p1.leftOuterJoin(p2).map(Task2_3.mapVal).map(lambda x: x[0]+','+str(x[1])+'\n')
        Task2_2.tofile(outfile, p.collect())
        os.system("rm -f tmp_task2_1.txt tmp_task2_2.txt")
        print('Duration:', time.time()-t0)
    
if __name__ == "__main__":
    conf = SparkConf().set("spark.sql.execution.arrow.enabled", "true")
    sc = SparkContext(conf=conf).getOrCreate()
    sc.setLogLevel("ERROR")
    Task2_3.run(sc, sys.argv[1], sys.argv[2], sys.argv[3])

    

    