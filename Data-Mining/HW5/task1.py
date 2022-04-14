# yifan
import time
import sys
import random
import binascii 
from blackbox import BlackBox

def Time(method):
    def timed(*args, **kw):
        ts = time.time()
        result = method(*args, **kw)
        te = time.time()
        print("Duration: %4.2f sec"%((te - ts)))
        return result
    return timed

def myhashs(x):
    funcs = []
    num_hash = 10
    for i in range(num_hash):
        funcs.append(Task1.hash_func(random.randint(1, sys.maxsize), random.randint(1, sys.maxsize), 27292996856087))
    hashed = []
    for func in funcs:
        hashed.append(func(x))
    return hashed

class Task1:
    def str2int(s):
        return int(binascii.hexlify(s.encode('utf8')),16)

    def hash_func(a, b, p, m=69997):
        def gen_func(x):
            if type(x) == str:
                x = Task1.str2int(x)
            return ((a * x + b) % p) % m
        return gen_func

    def one_run(stream):
        fp, tn = 0., 0.
        filters = set()
        prev = set()
        for s in stream:
            h = set(myhashs(s))
            if h.issubset(filters) == True and set(s).issubset(prev) == False:
                fp += 1
            if h.issubset(filters) == False and set(s).issubset(prev) == False:
                tn += 1
            filters |= h
            prev.add(s)
        return float(fp)/float((fp+tn))

if __name__ == "__main__":
    infile = sys.argv[1]
    stream_size = int(sys.argv[2])
    num_asks = int(sys.argv[3])
    outfile = sys.argv[4]
    t0 = time.time()
    with open(outfile, 'w+') as f:
        f.write("Time,FPR\n")
        for i in range(num_asks):
            stream = BlackBox().ask(infile, stream_size)
            fpr = Task1.one_run(stream)
            f.write(str(i)+','+str(fpr)+'\n')
    print("Duration:", time.time()-t0)
