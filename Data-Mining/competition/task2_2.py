import time
import sys
import json
import os
from pyspark import SparkContext, SparkConf
import xgboost as xgb

def Time(method):
    def timed(*args, **kw):
        ts = time.time()
        result = method(*args, **kw)
        te = time.time()
        print("   <RunTime> %s: %4.1f s"%(method.__name__, (te - ts)))
        return result
    return timed


class Task2_2:
    def get_val(x, key, fillval=0):
        mp = {"True":1., "False":0., 
            "average":1., "quiet":0., "very_loud":3.,"loud":2.}
        if key in x.keys():
            if x[key] in mp.keys():
                return (float)(mp[x[key]])
            return (float)(x[key])
        return (float)(fillval)

    def get_val1(x, key, fillval=-1):
        if key in x.keys():
            return (float)(x[key])
        return (float)(fillval)

    business_fix = [-1, -1] #+[-1,-1,-1,-1,-1,-1,-1,-1,-1,-1]  
    @Time
    def process_business(sc, file):
        def business_get_fea(x):
            bid, fea = x['business_id'], []
            fea.append(Task2_2.get_val(x, 'stars', -1))
            fea.append(Task2_2.get_val(x, 'review_count', -1))
            '''
            if 'attribute' in x.keys():
                fea.append(Task2_2.get_val(x['attribute'], 'BikeParking', -1))
                fea.append(Task2_2.get_val(x['attribute'], 'BusinessAcceptsCreditCards', -1))
                fea.append(Task2_2.get_val(x['attribute'], 'GoodForKids', -1))
                fea.append(Task2_2.get_val(x['attribute'], 'HasTV', -1))
                fea.append(Task2_2.get_val(x['attribute'], 'NoiseLevel', -1))
                fea.append(Task2_2.get_val(x['attribute'], 'OutdoorSeating', -1))
                fea.append(Task2_2.get_val(x['attribute'], 'RestaurantsPriceRange2', -1))
                fea.append(Task2_2.get_val(x['attribute'], 'RestaurantsGoodForGroups', -1))
                fea.append(Task2_2.get_val(x['attribute'], 'RestaurantsReservations', -1))
                fea.append(Task2_2.get_val(x['attribute'], 'RestaurantsTakeOut', -1))
            else:
                for i in range(10):
                    fea.append(-1)
            '''
            return (bid, fea)
        line = sc.textFile(file).map(lambda row: json.loads(row)) \
            .map(lambda x: business_get_fea(x))
        return line

    user_fix = [-1,-1,-1,-1]
    @Time
    def process_user(sc, file):
        def user_get_fea(x):
            uid, fea = x['user_id'], []
            fea.append(Task2_2.get_val(x, 'review_count', -1))
            fea.append(Task2_2.get_val(x, 'average_stars', -1))
            fea.append(Task2_2.get_val(x, 'fans', -1))
            fea.append(Task2_2.get_val(x, 'useful', -1))
            return uid, fea
        line = sc.textFile(file).map(lambda row: json.loads(row)) \
            .map(lambda x: user_get_fea(x))
        return line
    #business_fix += [-1]
    def process_photo(sc, file):
        line = sc.textFile(file).map(lambda row: json.loads(row)) \
            .map(lambda x: (x['business_id'], x['photo_id'])) \
            .groupByKey() \
            .map(lambda x: (x[0], [len(list(x[1]))]))
        return line#.collectAsMap()
    #business_fix += [-1]
    def process_checkin(sc, file):
        def merge_time(dic):
            s = 0.
            for k in dic.keys():
                s += dic[k]
            return s
        line = sc.textFile(file).map(lambda row: json.loads(row)) \
            .map(lambda x: (x['business_id'], merge_time(x['time']))) \
            .groupByKey() \
            .map(lambda x: (x[0], [sum(list(x[1]))]))
        return line#.collectAsMap()

    @Time
    def get_fea(sc, folder):
        business = Task2_2.process_business(sc, os.path.join(folder, 'business.json'))
        #photo = Task2_2.process_photo(sc, os.path.join(folder, 'photo.json'))
        #business = business.join(photo).mapValues(lambda x: x[0]+x[1])
        #checkin = Task2_2.process_checkin(sc, os.path.join(folder, 'checkin.json'))
        #business = business.join(checkin).mapValues(lambda x: x[0]+x[1])
        user = Task2_2.process_user(sc, os.path.join(folder, 'user.json'))
        return business, user

    def myConcatenate(a, b):
        list1, l1, list2, l2 = a[0], a[1], b[0], b[1]
        try:
            len(list1[0])
            try:
                len(list2[0])
                return (list1 + list2, l1+l2)
            except:
                list1.append(list2)
                return (list1, l1+l2)
        except:
            try:
                len(list2[0])
                list2.append(list1)
                return (list2, l2+l1)
            except:
                return ([list1,list2], l1+l2)

    def mapbusinesstrain(x, fix):
        if x[1][1] is None:
            return (x[1][0][0]+','+x[0], (fix,x[1][0][1]))
        assert len(fix) == len(x[1][1]), 'mapbusinesstrain '+str(len(x[1][1]))+' '+str(len(fix))
        return (x[1][0][0]+','+x[0], (x[1][1], x[1][0][1]))

    def mapusertrain(x, fix):
        if x[1][1] is None:
            return (x[0]+','+x[1][0][0], (fix,x[1][0][1]))
        assert len(fix) == len(x[1][1]), 'mapusertrain '+str(len(x[1][1]))+' '+str(len(fix))
        return (x[0]+','+x[1][0][0], (x[1][1], x[1][0][1]))

    def mapbusiness(x, fix):    
        if x[1][1] is None:
            return (x[1][0]+','+x[0], fix)
        assert len(fix) == len(x[1][1]), 'mapbusiness '+str(len(x[1][1]))+' '+str(len(fix))
        return (x[1][0]+','+x[0], x[1][1])

    def mapuser(x, fix):
        if x[1][1] is None:
            return (x[1][0]+','+x[0], fix)
        assert len(fix) == len(x[1][1]), 'mapuser '+str(len(x[1][1]))+' '+str(len(fix))
        return (x[0]+','+x[1][0], x[1][1])

    @Time
    def gen_train_data(sc, folder, business, user):
        rdd = sc.textFile(os.path.join(folder, 'yelp_train.csv'))
        fisrtline = rdd.first()
        data = rdd.filter(lambda line: line != fisrtline)
        # user_id, business_id, score
        #'bid', (('uid', score), fea))
        b = data.map(lambda line: (line.split(',')[1], (line.split(',')[0], (float)(line.split(',')[2])))).leftOuterJoin(business) \
            .map(lambda x: Task2_2.mapbusinesstrain(x, Task2_2.business_fix))
        u = data.map(lambda line: (line.split(',')[0], (line.split(',')[1], (float)(line.split(',')[2])))).leftOuterJoin(user) \
            .map(lambda x: Task2_2.mapusertrain(x, Task2_2.user_fix))
        # 'uid,bid', ((uf,s), (bf,s))
        fea = u.join(b).map(lambda x: (1, (x[1][0][0]+x[1][1][0], [x[1][0][1]]))) \
            .reduceByKey(lambda a, b: Task2_2.myConcatenate(a, b))
        fea = fea.collect()
        return fea[0][1]

    @Time
    def predict(sc, file, business, user, xbgr):
        def toformat(x):
            return x[0]+','+str(x[1][0])+'\n'
        rdd = sc.textFile(file)
        fisrtline = rdd.first()
        data = rdd.filter(lambda line: line != fisrtline)
        # user_id, business_id
        dic = rdd.map(lambda line: (line.split(',')[0]+','+line.split(',')[1]))\
                .zipWithIndex().collectAsMap()
        idic = {v: k for k, v in dic.items()}
        b = data.map(lambda line: (line.split(',')[1], line.split(',')[0])).leftOuterJoin(business) \
            .map(lambda x: Task2_2.mapbusiness(x, Task2_2.business_fix))
        # 'uid,bid', fea
        u = data.map(lambda line: (line.split(',')[0], line.split(',')[1])).leftOuterJoin(user) \
            .map(lambda x: Task2_2.mapuser(x, Task2_2.user_fix))
        pred = u.join(b).map(lambda x: (x[0], x[1][0]+x[1][1])).map(lambda x: (dic[x[0]], x[1]))
        pred = pred.map(lambda x: (x[0],x[1])).collect()
        key, val = [], []
        for p in pred:
            key.append(p[0])
            val.append(p[1])
        pred =  xbgr.predict(val)
        res = []
        for i in range(len(key)):
            res.append(idic[key[i]]+','+str(pred[i])+'\n')
        return res

    @Time
    def xbgr_fit(data):
        xbgr = xgb.XGBRegressor(n_jobs=10, nthread=10, n_estimators=80, max_depth=6)
        xbgr.fit(data[0], data[1])
        return xbgr

    @Time
    def tofile(file, res):
        with open(file, 'w+') as f:
            f.write('user_id,business_id,prediction\n')
            for line in res:
                f.write(line)

    def run(sc, folder, testfile, outfile, learner):
        t0 = time.time()
        business, user = Task2_2.get_fea(sc, folder)
        data = Task2_2.gen_train_data(sc, folder, business, user)
        xbgr = Task2_2.xbgr_fit(data)
        res = Task2_2.predict(sc, testfile, business, user, xbgr)
        Task2_2.tofile(outfile, res)
        if learner == True:
            res = Task2_2.predict(sc, os.path.join(folder, 'yelp_train.csv'), business, user, xbgr)
            Task2_2.tofile('train2.txt', res)
        print('Duration:', time.time()-t0)

if __name__ == "__main__":   
    conf = SparkConf().set("spark.sql.execution.arrow.enabled", "true")
    sc = SparkContext(conf=conf).getOrCreate()
    sc.setLogLevel("ERROR")
    Task2_2.run(sc, sys.argv[1], sys.argv[2], sys.argv[3])
    
