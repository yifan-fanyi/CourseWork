# yifan
import time
import sys
import random
import binascii 
from blackbox import BlackBox

def myhashs(x):
    funcs = []
    num_hash = 200
    for i in range(num_hash):
        funcs.append(Task2.hash_func(random.randint(1, sys.maxsize), random.randint(1, sys.maxsize), 27292996856087))
    hashed = []
    for func in funcs:
        hashed.append(func(x))
    return hashed

class Task2:
    def str2int(s):
        return int(binascii.hexlify(s.encode('utf8')),16)

    def hash_func(a, b, p, m=69997):
        def gen_func(x):
            if type(x) == str:
                x = Task2.str2int(x)
            return ((a * x + b) % p) % m
        return gen_func

    def one_run(stream):
        NUM_GROUP = 5
        GROUP_SIZE = int(200 / 5)
        longest_tzeros = [0 for _ in range(200)]
        for s in stream:
            h = myhashs(s)
            hash_str = list(map(lambda x: bin(x), h))
            tzeros = list(map(lambda x: len(x) - len(x.rstrip("0")), hash_str))
            longest_tzeros = [max(a, b) for a, b in zip(longest_tzeros, tzeros)]
        data = sorted([2 ** r for r in longest_tzeros])
        data = [data[GROUP_SIZE * i : GROUP_SIZE * (i + 1)] for i in range(NUM_GROUP)]
        avg = list(map(lambda x: sum(x) / len(x), data))
        med = int(len(avg) / 2)
        return round(avg[med])

if __name__ == "__main__":
    infile = sys.argv[1]
    stream_size = int(sys.argv[2])
    num_asks = int(sys.argv[3])
    outfile = sys.argv[4]
    previous_user = BlackBox().ask(infile, stream_size)
    t0 = time.time()
    with open(outfile, 'w+') as f:
        f.write("Time,FPR\n")
        for i in range(num_asks):
            stream = BlackBox().ask(infile, stream_size)
            fpr = Task2.one_run(stream)
            f.write(str(i)+','+str(stream_size)+','+str(fpr)+'\n')
    print("Duration:", time.time()-t0)
