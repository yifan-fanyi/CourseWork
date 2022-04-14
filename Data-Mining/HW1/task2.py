# @yifan
# 2022.01.26
import sys
import time
import json
from operator import add
from pyspark import SparkContext

from task1 import Task1

class Task2:
    def element_count(iterator):
        yield sum(1 for _ in iterator)

    def get_number_of_partitions(rdd):
        return rdd.getNumPartitions()
    
    def get_number_of_elements_in_each_partition(rdd):
        return rdd.mapPartitions(Task2.element_count).collect()

    def get_exe_time(rdd):
        t0 = time.time()
        Task1.top_10_businesses_had_largest_numbers_of_reviews_and_number_of_reviews_they_had(rdd)
        return time.time() - t0

    def partitioner(k):
        return ord(k[0])
    
if __name__ == '__main__':
    mySC = SparkContext.getOrCreate()
    save = {'default':{}, 'customized':{}}
    lines = mySC.textFile(sys.argv[1]).map(lambda line: json.loads(line))
    rdd = lines.map(lambda x: x['business_id'])
    save['default']['n_partition'] = Task2.get_number_of_partitions(rdd)
    save['default']['n_items']      = Task2.get_number_of_elements_in_each_partition(rdd)
    save['default']['exe_time']    = Task2.get_exe_time(rdd)


    rddc = lines.map(lambda x: (x['business_id'], x))
    rddc = rddc.partitionBy((int)(sys.argv[3]), Task2.partitioner)
    rddc = rddc.map(lambda x: x[0])

    save['customized']['n_partition'] = Task2.get_number_of_partitions(rddc)
    save['customized']['n_items']      = Task2.get_number_of_elements_in_each_partition(rddc)
    save['customized']['exe_time']    = Task2.get_exe_time(rddc)
    
    with open(sys.argv[2], 'w') as f:
        json.dump(save, f)