import time
import sys
import os
from pyspark import SparkContext, SparkConf
from task2_1 import Task2_1
from task2_2 import Task2_2
import xgboost as xbg
from sklearn.linear_model import LinearRegression
from os.path import exists
from sklearn.ensemble import RandomForestRegressor
#[('RMSE', 0.9819935864525267)]

class Task2_3:
    def mapVal(a, b):
        if abs(a-b) > 2:
            r = 0.01
        elif abs(a-b) > 1:
            r = 0.04
        elif abs(a-b) > 0.5:
            r = 0.11
        else:
            r = 0.13
        if a - b > 2:
            c = -0.16
        elif a-b < -2:
            c = 0.01
        elif a-b>1:
            c = 0.01
        elif a-b < -1:
            c = 0.01
        elif a-b>0:
            c = -0.007
        else:
            c = 0.007
        return r*a+(1.0-r)*b-c

    def xbgr_fit(x, y):
        print('fit xbgr')
        xbgr = RandomForestRegressor(n_jobs=-1)#xbg.XGBRegressor(n_jobs=10, nthread=10, n_estimators=80, max_depth=3)
        xbgr.fit(x, y)
        return xbgr

    def run(sc, folder, testfile, outfile, learner=False):
        force = True
        t0 = time.time()
        if exists('tmp_task2_1.txt') == False or force == True:
            if learner == True and exists('train1.txt') == False:
                Task2_1.run(sc, os.path.join(folder, 'yelp_train.csv'), testfile, 'tmp_task2_1.txt', folder, learner=True)
            else:
                Task2_1.run(sc, os.path.join(folder, 'yelp_train.csv'), testfile, 'tmp_task2_1.txt', folder, learner=False)
        if exists('tmp_task2_2.txt') == False or force == True:
            if learner == True and exists('train2.txt') == False:
                Task2_2.run(sc, folder, testfile, 'tmp_task2_2.txt', learner=True)
            else:
                Task2_2.run(sc, folder, testfile, 'tmp_task2_2.txt', learner=False)
        
        if learner == True:
            rdd = sc.textFile('train1.txt')
            fisrtline = rdd.first()
            train1 = rdd.filter(lambda line: line != fisrtline).map(lambda line: (line.split(',')[0]+','+line.split(',')[1], (float)(line.split(',')[2])))
            rdd = sc.textFile('train2.txt')
            fisrtline = rdd.first()
            train2 = rdd.filter(lambda line: line != fisrtline).map(lambda line: (line.split(',')[0]+','+line.split(',')[1], (float)(line.split(',')[2])))
            train = train1.leftOuterJoin(train2).map(lambda x: (x[0], [x[1][0], x[1][1]]))

            rdd = sc.textFile(os.path.join(folder, 'yelp_train.csv'))
            fisrtline = rdd.first()
            y = rdd.filter(lambda line: line != fisrtline).map(lambda line: (line.split(',')[0]+','+line.split(',')[1], (float)(line.split(',')[2])))
            train = train.leftOuterJoin(y).collect()
            x, y = [], []
            for i in train:
                x.append(i[1][0])
                y.append([i[1][1]])
            xbgr = Task2_3.xbgr_fit(x, y)
        rdd = sc.textFile('tmp_task2_1.txt')
        fisrtline = rdd.first()
        p1 = rdd.filter(lambda line: line != fisrtline).map(lambda line: (line.split(',')[0]+','+line.split(',')[1], (float)(line.split(',')[2])))
        rdd = sc.textFile('tmp_task2_2.txt')
        fisrtline = rdd.first()
        p2 = rdd.filter(lambda line: line != fisrtline).map(lambda line: (line.split(',')[0]+','+line.split(',')[1], (float)(line.split(',')[2])))
        p = p1.leftOuterJoin(p2).map(lambda x: (x[0], [x[1][0], x[1][1]])).collect()#.map(lambda x: x[0]+','+str(x[1])+'\n')
        a, b = [], []
        for i in p:
            a.append(i[0])
            b.append(i[1])
        if learner == True:
            pred = xbgr.predict(b).reshape(-1)
        
        with open(outfile, 'w+') as f:
            f.write('user_id,business_id,prediction\n')
            for i in range(len(a)):
                if learner == True:
                    f.write(a[i]+','+str(pred[i])+'\n')
                else:
                    f.write(a[i]+','+str(Task2_3.mapVal(b[i][0],b[i][1]))+'\n')
                
        #os.system("rm -f tmp_task2_1.txt tmp_task2_2.txt train1.txt train2.txt")
        print('Duration:', time.time()-t0)
    
if __name__ == "__main__":
    conf = SparkConf().set("spark.sql.execution.arrow.enabled", "true")
    sc = SparkContext(conf=conf).getOrCreate()
    sc.setLogLevel("ERROR")
    Task2_3.run(sc, sys.argv[1], sys.argv[2], sys.argv[3], False)

    

    