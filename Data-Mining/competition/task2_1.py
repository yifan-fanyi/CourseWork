
# yifan
from asyncio import Task
from re import M
import time
import sys
import math
import os
import random
from pyspark import SparkContext
from collections import defaultdict

class Task2_1:
    def genHash(a, b, m):
        def myHash(x):
            return (a*x+b) % m
        return myHash
    
    def genHashs(n_funcs, baskets):
        a = random.sample(range(1, sys.maxsize-1), n_funcs)
        b = random.sample(range(1, sys.maxsize-1), n_funcs)
        funcs = []
        for i in range(n_funcs):
            funcs.append(Task2_1.genHash(a[i], b[i], baskets))
        return funcs

    def average(v):
        a, b = sum(v), len(v)
        if b == 0:
            return 0
        return a / b

    def PearsonSimilarity(dict1, dict2):
        co_rated = list(set(dict1.keys()) & (set(dict2.keys())))
        v1, v2 = list(), list()
        for user in co_rated:
            v1.append(dict1[user])
            v2.append(dict2[user])
        avg1, avg2 = Task2_1.average(v1), Task2_1.average(v2)
        x, y0, y1 = 0., 0., 0.
        for i in range(len(v1)):
            x += (v1[i]-avg1)*(v2[i]-avg2)
            y0 += (v1[i]-avg1)**2
            y1 += (v2[i]-avg2)**2
        if y0 < 1e-10 or y1 < 1e-10:
            return 0.
        return x / math.sqrt(y0*y1)

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
                s = Task2_1.Jaccard(data.get(pair[0], set()),
                                  data.get(pair[1], set()))
                if s >= TH:
                    p = [imp[pair[0]],imp[pair[1]]]
                    p.sort()
                    res.append(p[0]+','+p[1]+','+str(s))
        res.sort()
        return res
    
    def toFile(file, data):
        data.sort()
        with open(file, 'w+') as f:
            f.write('user_id,business_id,prediction\n')
            for line in data:
                f.write(line + "\n")

    def dictmerge(dict_list):
        res = defaultdict(int)
        for d in dict_list:
            for k in d.keys():
                res[k] = d[k]
        return res

    def toformat(x, y, s):
        return x+','+y+','+str(s)

    def rdd_get(rdd, key, fillval=0):
        try:
            a = rdd.filter(lambda x: x[0] == key)
            return a.collect()[0][1]
        except:
            return fillval

    def predict_item_based(x, n_neighbours, business_mean_score, model, ibusiness_dict, fillval=3.7511703308515445):
        # fillval=3.7511703308515445 mean score of all the training stars
        target = x[0]  # bidx
        busines_score = list(x[1])  # list of tuple(bidx, score)
        result = list()
        for a in busines_score:
            if target < a[0]:
                key = tuple((target, a[0]))
            else:
                key = tuple((a[0], target))
            try:
                sim = Task2_1.PearsonSimilarity(model[key[0]], model[key[1]])
            except:
                continue
            result.append(tuple((a[1], sim)))
        score_sim = sorted(result, key=lambda x: x[1], reverse=True)[:n_neighbours]
        num = sum(map(lambda item: item[0] * item[1], score_sim))
        if num <= 0:
            return tuple((target, business_mean_score.get(ibusiness_dict[target], fillval)))
        den = sum(map(lambda item: abs(item[1]), score_sim))
        if den == 0:
            return tuple((target, business_mean_score.get(ibusiness_dict[target], fillval)))
        
        return tuple((target, num / den))

    def fillmeanval(user_mean, business_mean, user_id, business_id, fillval):
        a = user_mean.get(user_id, fillval)
        if a != fillval:
            return a
        b = business_mean.get(business_id, fillval)
        return b

    def run(sc, trainfile, testfile, outfile, folder=None, learner=False):
        t0 = time.time()
        min_Co_Rate = 200
        n_neighbours = 8
        fillval = 3.7511703308515445 # avg of all stars
        

        rdd = sc.textFile(trainfile)
        fisrtline = rdd.first()
        data = rdd.filter(lambda line: line != fisrtline)
        # user_id, business_id, score
        data = data.map(lambda line: (line.split(',')[0], line.split(',')[1], (float)(line.split(',')[2])))
        
        business_mean_score = data.map(lambda x: (x[1], x[2])) \
            .groupByKey() \
            .map(lambda x: (x[0], (sum(x[1]), len(list(x[1]))))) \
            .reduceByKey(lambda a, b: (a[0] + b[0], a[1] + b[1])) \
            .mapValues(lambda value: float(value[0] / value[1])).collectAsMap()

        user_mean_score = data.map(lambda x: (x[0], x[2])) \
            .groupByKey() \
            .map(lambda x: (x[0], (sum(x[1]), len(list(x[1]))))) \
            .reduceByKey(lambda a, b: (a[0] + b[0], a[1] + b[1])) \
            .mapValues(lambda value: float(value[0] / value[1])).collectAsMap()
        
        if fillval < 0:
            fillval = data.map(lambda x: (1, x[2])).groupByKey() \
                .map(lambda x: (x[0], (sum(x[1]), len(list(x[1]))))) \
                .reduceByKey(lambda a, b: (a[0] + b[0], a[1] + b[1])) \
                .mapValues(lambda value: float(value[0] / value[1])).collect()
            fillval = fillval[1]

        # user_id: idx
        user = data.map(lambda x: x[0]).distinct() \
                .sortBy(lambda item: item) \
                .zipWithIndex() \
                .map(lambda x: {x[0]: x[1]}) \
                .flatMap(lambda x: x.items())
        user_dict = user.collectAsMap()
        iuser_dict = {v: k for k, v in user_dict.items()}
        # business_id: idx
        business = data.map(lambda x: x[1]).distinct() \
                .sortBy(lambda item: item) \
                .zipWithIndex() \
                .map(lambda x: {x[0]: x[1]}) \
                .flatMap(lambda x: x.items())
        business_dict = business.collectAsMap()
        ibusiness_dict = {v: k for k, v in business_dict.items()}
        # user, (business,score)
        train_data = data.map(lambda x: (x[0], (x[1],x[2]))).groupByKey() \
            .map(lambda x: (user_dict[x[0]],[(business_dict[xx[0]], xx[1]) for xx in list(set(x[1]))]))

        # business_id: [(user_id,score), (user_id,score),...]
        business_user = data \
            .map(lambda x: (business_dict[x[1]], (user_dict[x[0]], x[2]))) \
            .groupByKey() \
            .map(lambda x: list(x)) \
            .filter(lambda x: len(x[1]) >= min_Co_Rate) \
            .mapValues(lambda x: [{xx[0]: xx[1]} for xx in x]) \
            .mapValues(lambda x: Task2_1.dictmerge(x))
        business_user_dict =  business_user \
            .map(lambda x: {x[0]: x[1]}) \
            .flatMap(lambda x: x.items()).collectAsMap()
        cand_business = business_user.map(lambda x: x[0])
        rdd = sc.textFile(testfile)
        fisrtline = rdd.first()
        rdd = rdd.filter(lambda line: line != fisrtline)
        rdd =  rdd\
            .map(lambda line: (user_dict.get(line.split(',')[0],line.split(',')[0]), business_dict.get(line.split(',')[1], line.split(',')[1]))) 
        test_data = rdd.filter(lambda x: (type(x[0]) is not str) and (type(x[1]) is not str))
        
        # NOT APPEARED USER / BUSINESS   use mean score as pred    
        res0 = rdd.filter(lambda x: (type(x[0]) is str) or (type(x[1]) is str)) \
            .map(lambda x: Task2_1.toformat(iuser_dict.get(x[0],x[0]),
                                            ibusiness_dict.get(x[1], x[1]),
                                            Task2_1.fillmeanval(user_mean_score,
                                                                business_mean_score,
                                                                iuser_dict.get(x[0],x[0]),
                                                                ibusiness_dict.get(x[1], x[1]),
                                                                fillval))).collect()
    # res = test_data.leftOuterJoin(train_data)
        res = test_data.leftOuterJoin(train_data)  \
                .mapValues(lambda x: Task2_1.predict_item_based(x=tuple(x),
                                                        n_neighbours=n_neighbours,
                                                        business_mean_score=business_mean_score,
                                                        model=business_user_dict,
                                                        ibusiness_dict=ibusiness_dict,
                                                        fillval=fillval)) \
                .map(lambda x: Task2_1.toformat(iuser_dict[x[0]],
                                                ibusiness_dict[x[1][0]],
                                                x[1][1])).collect()
        Task2_1.toFile(outfile, res0+res)
        if learner == True:
            rdd = sc.textFile(os.path.join(folder, 'yelp_train.csv'))
            fisrtline = rdd.first()
            rdd = rdd.filter(lambda line: line != fisrtline)
            rdd =  rdd\
                .map(lambda line: (user_dict.get(line.split(',')[0],line.split(',')[0]), business_dict.get(line.split(',')[1], line.split(',')[1]))) 
            test_data = rdd.filter(lambda x: (type(x[0]) is not str) and (type(x[1]) is not str))
            
            # NOT APPEARED USER / BUSINESS   use mean score as pred    
            res0 = rdd.filter(lambda x: (type(x[0]) is str) or (type(x[1]) is str)) \
                .map(lambda x: Task2_1.toformat(iuser_dict.get(x[0],x[0]),
                                                ibusiness_dict.get(x[1], x[1]),
                                                Task2_1.fillmeanval(user_mean_score,
                                                                    business_mean_score,
                                                                    iuser_dict.get(x[0],x[0]),
                                                                    ibusiness_dict.get(x[1], x[1]),
                                                                    fillval))).collect()
        # res = test_data.leftOuterJoin(train_data)
            res = test_data.leftOuterJoin(train_data)  \
                    .mapValues(lambda x: Task2_1.predict_item_based(x=tuple(x),
                                                            n_neighbours=n_neighbours,
                                                            business_mean_score=business_mean_score,
                                                            model=business_user_dict,
                                                            ibusiness_dict=ibusiness_dict,
                                                            fillval=fillval)) \
                    .map(lambda x: Task2_1.toformat(iuser_dict[x[0]],
                                                    ibusiness_dict[x[1][0]],
                                                    x[1][1])).collect()
            Task2_1.toFile('train1.txt', res0+res)
        print("Duration:",time.time()-t0)

if __name__ == "__main__":
    sc = SparkContext.getOrCreate()
    sc.setLogLevel("ERROR")
    Task2_1.run(sc, sys.argv[1], sys.argv[2], sys.argv[3])