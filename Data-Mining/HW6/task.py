# yifan
from os import ctermid
import time
import sys
import numpy as np
import copy
from sklearn import cluster
from sklearn.model_selection import train_test_split

class myKMeans():
    def __init__(self, n_clusters=-1):
        self.KM = cluster.KMeans(  n_clusters=n_clusters , random_state=10)
        self.cluster_centers_ = []
    
    def fit(self, X):
        X = X.reshape(  -1, X.shape[-1]  )
        self.KM.fit(  X  )
        self.cluster_centers_ = copy.deepcopy(np.array(  self.KM.cluster_centers_  )).astype('float32')
        return self

    def predict(self, X):
        S = (list)(X.shape)
        X = X.astype('float32')
        S[-1] = -1
        X = X.reshape(-1, X.shape[-1])
        idx = self.KM.predict(X)
        return idx.reshape(S)

def Mahalanobis_Dist(X1, X2, std):
    return float(np.sqrt(sum([((a - b) / sd) ** 2 for (a, b, sd) in zip(X1, X2, std)])))

def init(lb, xr, n_clusters):
    RS, DS, CS = {}, {'N':[], 'SUM':[], 'SUMSQ':[], 'lb':[]}, {'N':[], 'SUM':[], 'SUMSQ':[], 'lb':[]}
    # step 2
    km = myKMeans(min(5 * n_clusters, xr.shape[0]//2)).fit(xr)
    # step 3
    l = km.predict(xr).reshape(-1)
    idx = np.zeros(l.shape[0])
    for i in range(min(5 * n_clusters, xr.shape[0]//2)):
        tidx = l == i
        if np.sum(tidx) < 2:
            assert np.sum(tidx) > 0, "error"
            idx += tidx.astype('float32')
    RS = {'lb':np.array(lb)[idx>0].tolist(), 'xr':xr[idx>0]}
    # step 4
    lb, xr = np.array(lb)[idx < 1].tolist(), xr[idx < 1]
    km = myKMeans(n_clusters).fit(xr)
    # step 5
    l = km.predict(xr).reshape(-1)
    for i in range(n_clusters):
        tidx = l == i
        DS['N'].append(np.sum(tidx))
        DS['SUM'].append(np.sum(xr[tidx], axis=0))
        DS['SUMSQ'].append(np.sum(np.square(xr[tidx]), axis=0))
        DS['lb'].append(np.array(lb)[tidx].tolist())
    # step 6
    lb, xr = RS['lb'], RS['xr']
    km = myKMeans(min(5 * n_clusters, xr.shape[0]//2)).fit(xr)
    l = km.predict(xr).reshape(-1)
    idx = np.zeros(l.shape[0])
    for i in range(min(5 * n_clusters, xr.shape[0]//2)):
        tidx = l == i
        if np.sum(tidx) < 2:
            assert np.sum(tidx) > 0, "error"
            idx += tidx
        else:
            CS['N'].append(np.sum(tidx))
            CS['SUM'].append(np.sum(xr[tidx], axis=0))
            CS['SUMSQ'].append(np.sum(np.square(xr[tidx]), axis=0))
            CS['lb'].append(np.array(lb)[tidx].tolist())
    RS = {'lb':np.array(lb)[idx>0].tolist(), 'xr':xr[idx>0]}
    return RS, DS, CS

def oneiter(lb, xr, RS, DS, CS, n_clusters):
    # step 8
    tlb, txr = [], []
    a, b = [], []
    for j in range(len(DS['SUM'])):
        a.append(DS['SUM'][j]/DS['N'][j])
        b.append(np.sqrt(DS['SUMSQ'][j]/DS['N'][j] - (DS['SUM'][j]/DS['N'][j])**2))
    for i in range(xr.shape[0]):
        dis = []
        for j in range(len(DS['SUM'])):
            dis.append(Mahalanobis_Dist(xr[i], a[j], b[j]))
        if np.min(dis) < 2 * np.sqrt(xr.shape[1]):
            jj = np.argmin(dis)
            DS['N'][jj] += 1
            DS['SUM'][jj] += xr[i]
            DS['SUMSQ'][jj] += np.square(xr[i])
            DS['lb'][jj].append(lb[i])
        else:
            tlb.append(lb[i])
            txr.append(xr[i])
    # step 9
    lb, xr = np.array(lb), np.array(xr)
    tlb, txr = [], []
    a, b = [], []
    for j in range(len(CS['SUM'])):
        a.append(CS['SUM'][j]/CS['N'][j])
        b.append(np.sqrt(CS['SUMSQ'][j]/CS['N'][j] - (CS['SUM'][j]/CS['N'][j])**2))
    for i in range(xr.shape[0]):
        dis = []
        for j in range(len(CS['SUM'])):
            dis.append(Mahalanobis_Dist(xr[i], a[j], b[j]))
        if np.min(dis) < 2 * np.sqrt(xr.shape[1]):
            jj = np.argmin(dis)
            CS['N'][jj] += 1
            CS['SUM'][jj] += xr[i]
            CS['SUMSQ'][jj] += np.square(xr[i])
            CS['lb'][jj].append(lb[i])
        else:
            tlb.append(lb[i])
            txr.append(xr[i])
    # step 10
    lb, xr = tlb+ RS['lb'], np.concatenate([txr, RS['xr']], axis=0)
    # step 11
    tCS = {'N':[], 'SUM':[], 'SUMSQ':[], 'lb':[]}
    km = myKMeans(min(5 * n_clusters, xr.shape[0]//2)).fit(xr)
    l = km.predict(xr).reshape(-1)
    idx = np.zeros(l.shape[0])
    for i in range(min(5 * n_clusters, xr.shape[0]//2)):
        tidx = l == i
        if np.sum(tidx) < 2:
            assert np.sum(tidx) > 0, "error"
            idx += tidx
        else:
            tCS['N'].append(np.sum(tidx))
            tCS['SUM'].append(np.sum(xr[tidx], axis=0))
            tCS['SUMSQ'].append(np.sum(np.square(xr[tidx]), axis=0))
            tCS['lb'].append(np.array(lb)[tidx].tolist())
    RS = {'lb':np.array(lb)[idx>0].tolist(), 'xr':xr[idx>0]}
    # step 12
    e = len(CS['N'])
    for j in range(len(CS['SUM'])):
        a.append(CS['SUM'][j]/CS['N'][j])
        b.append(np.sqrt(CS['SUMSQ'][j]/CS['N'][j] - (CS['SUM'][j]/CS['N'][j])**2))
    for i in range(len(tCS['N'])):
        dis = []
        c = tCS['SUM'][i]/tCS['N'][i]
        for j in range(e):            
            dis.append(Mahalanobis_Dist(c, 
                                        a[j], 
                                        b[j]))
        if np.min(dis) < 2 * np.sqrt(xr.shape[1]):
            jj = np.argmin(dis)
            CS['N'][jj] += tCS['N'][i]
            CS['SUM'][jj] += tCS['SUM'][i]
            CS['SUMSQ'][jj] += tCS['SUMSQ'][i]
            CS['lb'][jj] += tCS['lb'][i]
        else:
            CS['N'].append(tCS['N'][i])
            CS['SUM'].append(tCS['SUM'][i])
            CS['SUMSQ'].append(tCS['SUMSQ'][i])
            CS['lb'].append(tCS['lb'][i])
    return RS, DS, CS

def get_log(r, RS, DS, CS):
    st = "Round "+str(r)+': '
    c = 0
    for i in range(len(DS['N'])):
        c += DS['N'][i]
    st += str(c) +','
    st += str(len(CS['N'])) +','
    c = 0
    for i in range(len(CS['N'])):
        c += CS['N'][i]
    st += str(c) +','
    st += str(len(RS['xr'])) + '\n'
    return st

if __name__ == "__main__":
    t0 = time.time()
    X = np.loadtxt(sys.argv[1], dtype=float, delimiter = ",").astype('float32')
    n_clusters = (int)(sys.argv[2])
    batch_size = X.shape[0] // 5 + 1
    s = 0
    # step 1
    xr = X[s:s+batch_size]
    s += batch_size
    # step 2-6
    with open(sys.argv[3], 'w+') as f:
        f.write("The intermediate results:\n")
        RS, DS, CS = init(xr[:,0].tolist(), xr[:, 2:], n_clusters)
        f.write(get_log(1, RS, DS, CS))
    
    # step 7
        for i in range(4):
            xr = X[s:s+batch_size]
            s += batch_size
            RS, DS, CS = oneiter(xr[:,0].tolist(), xr[:, 2:], RS, DS, CS, n_clusters)
            if i < 3:
                f.write(get_log(1+i, RS, DS, CS))
        # merge CS DS
        tCS = {'N':[], 'SUM':[], 'SUMSQ':[], 'lb':[]}
        e = len(DS['N'])
        for i in range(len(CS['N'])):
            dis = []
            for j in range(e):            
                dis.append(Mahalanobis_Dist(CS['SUM'][i]/CS['N'][i], 
                                            DS['SUM'][j]/DS['N'][j], 
                                            np.sqrt(DS['SUMSQ'][j]/DS['N'][j] - (DS['SUM'][j]/DS['N'][j])**2)))
            if np.min(dis) < 2 * np.sqrt(xr.shape[1]):
                jj = np.argmin(dis)
                DS['N'][jj] += CS['N'][i]
                DS['SUM'][jj] += CS['SUM'][i]
                DS['SUMSQ'][jj] += CS['SUMSQ'][i]
                DS['lb'][jj] += CS['lb'][i]
            else:
                tCS['N'].append(CS['N'][i])
                tCS['SUM'].append(CS['SUM'][i])
                tCS['SUMSQ'].append(CS['SUMSQ'][i])
                tCS['lb'].append(CS['lb'][i])
        CS = tCS
        f.write(get_log(5, RS, DS, CS))

        label = np.zeros(X.shape[0])
        for i in range(len(RS['lb'])):
            label[(int)(RS['lb'][i])] = -1
        ct = 0
        for i in range(len(DS['lb'])):
            for j in range(len(DS['lb'][i])):
                label[(int)(DS['lb'][i][j])] = ct
            ct += 1
        for i in range(len(CS['lb'])):
            for j in range(len(CS['lb'][i])):
                label[(int)(CS['lb'][i][j])] = ct
            ct += 1
        label = label.astype('int16')
        f.write('\n')
        f.write("The clustering results:\n")
        for i in range(len(label)):
            f.write(str(i)+','+str(label[i])+'\n')
    print('Duration:',time.time()-t0)