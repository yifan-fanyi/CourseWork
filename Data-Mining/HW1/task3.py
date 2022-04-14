# @yifan
# 2022.01.26
import sys
import time
import json
from operator import add
from pyspark import SparkContext

class Task3:
    def avg_star(star_rdd, city_rdd):
        # (x['business_id'], x['stars']))
        # (x['business_id'], x['city']))
        star_rdd = star_rdd.groupByKey().map(
                lambda x: (x[0], ((float)(sum(x[1])), (float)(len(x[1])))))
        rdd = city_rdd.leftOuterJoin(star_rdd) # business id, (city, (sum_score, ct_score))
        rdd = rdd.map(lambda x: x[1]) # (city, (sum_score, ct_score)
        rdd = rdd.filter(lambda x: x[1] is not None and x[1] != "")
        rdd = rdd.reduceByKey(lambda a, b: (a[0] + b[0], a[1] + b[1]))
        #print(rdd.collect())
        rdd = rdd.mapValues(lambda value: float(value[0] / value[1]))
        return rdd.sortBy(lambda x:(-x[1], x[0])).collect()

    def avg_star_method2(star_rdd, city_rdd):
        star_rdd = star_rdd.groupByKey().map(
                lambda x: (x[0], ((float)(sum(x[1])), (float)(len(x[1])))))
        city_rdd = city_rdd.filter(
            lambda c: (c[1] is not None) and (c[1] != "")).mapValues(
                lambda values: [v.strip() for v in values.split(',')])
        rdd = city_rdd.leftOuterJoin(star_rdd)
        return rdd.map(
            lambda x: x[1]).filter(
                lambda x: x[1] is not None and x[1] != "").flatMap(
                    lambda x: [(city, x[1]) for city in x[0]]).reduceByKey(
                        lambda a, b: (a[0] + b[0], a[1] + b[1])).mapValues(
                            lambda value: float(value[0] / value[1])).takeOrdered(
                                10, key=lambda x: -x[1])
    
    def avg_star_method1(f1, f2):
        def load_file(fn, keys):
            dic = {}
            for k in keys:
                dic[k] = []
            d = open(fn, encoding="utf8").readlines()
            for dd in d:
                dd = json.loads(dd)
                for k in keys:
                    if type(dd[k]) == str:
                        dd[k] = dd[k].upper()  
                    dic[k].append(dd[k])
            return dic
        review = load_file(f1, ['business_id', 'stars'])
        city = load_file(f2, ['business_id', 'city'])
        def reduce(X, basedon='business_id', col='stars'):
            dic = {}
            for i in range(len(X[basedon])):
                if X[basedon][i] not in dic.keys():
                    dic[X[basedon][i]] = [X[col][i]]
                else:
                    dic[X[basedon][i]].append(X[col][i])
            return dic
        review = reduce(review, basedon='business_id', col='stars')
        city = reduce(city, basedon='city', col='business_id')
        res = []
        for c in city.keys():
            tmp = []
            for i in city[c]:
                try:
                    tmp += review[i]
                except:
                    pass
            if len(tmp) > 0:
                a = (float)(sum(tmp)) / (float)(len(tmp))
            else:
                a = 0
            res.append([c,a])
        return sorted(res, key=lambda x:-x[1])[:10]

if __name__ == '__main__':
    sc = SparkContext.getOrCreate()
    review_lines = sc.textFile(sys.argv[1]).map(lambda line: json.loads(line))
    business_lines = sc.textFile(sys.argv[2]).map(lambda line: json.loads(line))
    rdd1 = review_lines.map(lambda x: (x['business_id'], x['stars']))
    rdd2 = business_lines.map(lambda x: (x['business_id'], x['city']))
    res = Task3.avg_star(rdd1, rdd2)
    with open(sys.argv[3], 'w') as f:
        f.write("city,stars\n")
        for i in range(len(res)):
            f.write("%s,%1.1f"%(res[i][0], res[i][1]))
            if i < len(res)-1:
                f.write('\n')
    save = {}
    t0 = time.time()
    #print(Task3.avg_star_method2(rdd1, rdd2))
    save['m2'] = time.time() - t0

    t0 = time.time()
    #print(Task3.avg_star_method1(sys.argv[1], sys.argv[2]))
    save['m1'] = time.time() - t0
    save['reason'] = 'If the amount of data is small, method 1 would run faster since it do not requires to split the data and combine them among different works. Howvever, if the amount of data is huge, method2 would run faster since spark is designed for large amount of data.'
    with open(sys.argv[4], 'w') as f:
        json.dump(save, f)