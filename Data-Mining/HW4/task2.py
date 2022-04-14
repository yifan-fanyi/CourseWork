# yifan
from functools import _make_key
import time
import sys
import math
from operator import add
import random
import itertools
from tkinter import W
from pyspark.sql import SparkSession
from pyspark import SparkContext
from collections import defaultdict
from itertools import combinations

def Time(method):
    def timed(*args, **kw):
        ts = time.time()
        result = method(*args, **kw)
        te = time.time()
        print("   <RunTime> %s: %4.6f s"%(method.__name__, (te - ts)))
        return result
    return timed

class myNode():
    def __init__(self):
        self.current_node = None
        self.parent_node = []
        self.children_node = []
        self.weight = 1.0
        self.level = 0
        self.n_shortest = 0.

    def set(self, current_node, parent_node, children_node, level=-1, n_shortest=-1):
        if current_node is not None:
            self.current_node = current_node
        if parent_node is not None:
            self.parent_node += parent_node
            #self.parent_node = list(set(self.parent_node))
        if children_node is not None:
            self.children_node += children_node
            #self.children_node = list(set(self.children_node))
        if level > 0:
            self.level = level
        if n_shortest > 0:
            self.n_shortest += n_shortest
        return self

    def add_weight(self, val):
        self.weight += float(val)
        return self

class Task2:
    def makekey(p0, p1):
        a = sorted([p0,p1])
        return '(\''+a[0]+'\', \''+a[1]+'\')'

    def get_v_e(sc, TH, infile):
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
                edge.append([pair[0], pair[1]])
                vertex.add(pair[0])
                vertex.add(pair[1])
        return list(vertex), edge

    def find_children(parent, edges, visited):
        children = []
        for i in range(len(edges)):
            key = Task2.makekey(edges[i][0], edges[i][1])
            if visited[key] < 1: # not visited
                if edges[i][0] == parent:
                    children.append(edges[i][1])
                    visited[key] = 1
                if edges[i][1] == parent:
                    children.append(edges[i][0])
                    visited[key] = 1
        return children, visited

    def build_tree(root_node, edges):
        visited, tree, level = defaultdict(int), defaultdict(myNode), 0
        tree[root_node].set(current_node=root_node, parent_node=None, children_node=None, level=0, n_shortest=1)
        level += 1
        needvisit, cache = [root_node], []
        v = defaultdict(int)
        while len(needvisit) > 0:
            current = needvisit[0]
            v[current] = 1
            needvisit.pop(0)            
            children, visited = Task2.find_children(current, edges, visited)
            tree[current].set(current_node=None, parent_node=None, children_node=children) # set children
            for child in children:
                tree[child].set(current_node=child, parent_node=[current], children_node=None, level=level, n_shortest=tree[current].n_shortest) # set parent for each child
            cache += children
            if len(needvisit) == 0: # finish one level start next level
                for i in range(len(cache)):
                    for j in range(i+1, len(cache)):
                        visited[Task2.makekey(cache[i], cache[j])] = 1 # disable all inter level connection
                needvisit += cache
                cache = []
                level += 1
        return tree, level-2

    def traverse(tree, level):
        result = defaultdict(float)
        needvisit, cache = [], []
        for k in tree.keys():
            if len(tree[k].children_node) == 0 and tree[k].level == level:
                needvisit.append(k)
        while len(needvisit) > 0:
            current = needvisit[0]
            needvisit.pop(0)
            cache += tree[current].parent_node
            for kk in tree[current].parent_node:
                w = (float)(tree[current].weight) * (float)(tree[kk].n_shortest) / (float)(tree[current].n_shortest)
                tree[kk].add_weight(w)
                result[Task2.makekey(current, kk)] = w
            if len(needvisit) == 0:
                needvisit += cache
                cache = []
                level -= 1
                for k in tree.keys():
                    if len(tree[k].children_node) == 0 and tree[k].level == level:
                        needvisit.append(k)
                needvisit = list(set(needvisit))
        return result

    def tree_unit():
        v = ['a', 'b', 'c', 'd', 'e', 'f','g']
        e = [['a','b'], ['a','c'],['b','e'],['b','d'],['g','f']]
        tree, level = Task2.build_tree('a', e)
        res = Task2.traverse(tree, level)
        print(res)
        edges = []
        for x in v:
            edges.append(Task2.one_node(x, e))
        res = defaultdict(float)
        for d in edges:
            res = Task2.dictmerge(res, d)
        res = {k: v for k, v in sorted(res.items(), key=lambda item: (-round(item[1]/2, 5),item[0]))}
        for i in res.keys():
            print(i+','+str(round(res[i]/2, 5)))
        return res

    def one_node(root_node, edges):
        tree, level = Task2.build_tree(root_node, edges)
        res = Task2.traverse(tree, level)
        return res

    def dictmerge(a, b):
        for k in a.keys():
            b[k] += a[k]
        return b

    @Time
    def betweenness(sc, vertex, edges, outfile):
        t0 = time.time()
        edges = sc.parallelize(vertex).map(lambda x: Task2.one_node(x, edges)).collect()
        print(time.time()-t0)
        res = defaultdict(int)
        for edge in edges:
            res = Task2.dictmerge(res, edge)
        res = {k: v for k, v in sorted(res.items(), key=lambda item: (-round(item[1]/2, 5),item[0]))}
        if outfile is not None:
            with open(outfile, 'w+') as f:
                for i in res.keys():
                    f.write(i+','+str(round(res[i]/2, 5))+'\n')
        return res

    def find_Communities(vertexs, edges):
        communities = []
        visited = defaultdict(int)
        visited_count = 0
        while visited_count < len(vertexs):
            for v in vertexs:
                if visited[v] == 0:
                    root = v
                    break
            cache = [root]
            com = []
            while len(cache) > 0:
                v = cache[0]
                cache.pop(0)
                if visited[v] == 1:
                    continue
                visited[v] = 1
                visited_count +=1
                com.append(v)
                for e in edges:
                    if v == e[0]:
                        if visited[e[1]] < 1:
                            cache.append(e[1])
                    if v == e[1]:
                        if visited[e[0]] < 1:
                            cache.append(e[0])
            communities.append(list(set(com)))
        for i in range(len(communities)):
            communities[i] = sorted(communities[i])
        communities.sort(key = lambda x:(len(x),x))
        return communities

    def degree(vertexs, edges):
        d = defaultdict(int)
        for v in vertexs:
            for e in edges:
                if v == e[0] or v == e[1]:
                    d[v] += 1
        return d

    @Time
    def modularity(vertexs, edges, deg, ori_n_edge, be):
        communities = Task2.find_Communities(vertexs, edges)
        temp_sum = 0.
        for cluster in communities:
            for i in list(cluster):
                k_i = deg[i]
                for j in list(cluster):         
                    k_j = deg[j]
                    if Task2.makekey(i,j) in be.keys():
                        A = 1.
                    else:
                        A = 0.
                    temp_sum += float(A - (k_i * k_j / (2 * ori_n_edge)))
        return communities, float(temp_sum / (2 * ori_n_edge))

    @Time
    def remove(edge, be):
        ck, m = [], 0
        for k in be.keys():
            if be[k] > m:
                ck = [k]
                m = be[k]
            if be[k] == m:
                ck.append(k)
        tmp = []
        for i in range(len(edge)):
            if Task2.makekey(edge[i][0], edge[i][1]) not in ck:
                tmp.append(edge[i])
        return tmp, len(ck)

    @Time
    def find_max_modularity(sc, vertex, edge, deg, beori, ori_n_edge):
        edge, ct = Task2.remove(edge, beori)
        com, m = Task2.modularity(vertex, edge, deg, ori_n_edge, beori)

        while ct < ori_n_edge:
            be = Task2.betweenness(sc, vertex, edge, None)
            edge, ctc = Task2.remove(edge, be)
            comc, mc = Task2.modularity(vertex, edge, deg, ori_n_edge, beori)
            ct += ctc
            if mc > m:
                com = comc
                m = mc
        print(m)
        return com, m
        
    @Time
    def run(sc, TH, infile, outfile_b, outfile_c):
        vertex, edges = Task2.get_v_e(sc, TH, infile)
        be = Task2.betweenness(sc, vertex, edges, outfile_b)
        deg = Task2.degree(vertex, edges)
        ori_n_edge = len(edges)
        com, m = Task2.find_max_modularity(sc, vertex, edges, deg, be, ori_n_edge)
        with open(outfile_c, 'w+') as f:
            for line in com:
                f.write(str(line)[1:-1] + "\n")

if __name__ == "__main__":
    sc = SparkContext.getOrCreate()
    spark = SparkSession(sc)
    sc.setLogLevel("ERROR")
    Task2.run(sc, (float)(sys.argv[1]), sys.argv[2], sys.argv[3], sys.argv[4])
    
    #Task2.tree_unit()