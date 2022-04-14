# yifan
import time
import sys
import random
from blackbox import BlackBox     
    
if __name__ == "__main__":
    random.seed(553) 
    infile = sys.argv[1]
    stream_size = int(sys.argv[2])
    num_asks = int(sys.argv[3])
    outfile = sys.argv[4]
    t0 = time.time()
    n = 0
    USERS = []
    with open(outfile, 'w+') as f:
        f.write("seqnum,0_id,20_id,40_id,60_id,80_id\n")
        for k in range(num_asks):
            stream = BlackBox().ask(infile, stream_size)
            for i in range(len(stream)):
                n += 1
                if len(USERS) < 100:
                    USERS.append(stream[i])
                else:
                    if random.random() < float(100)/float(n):
                        b = random.randint(0, 99)
                        USERS[b] = stream[i]

            f.write(str(n)+','+USERS[0]+','+USERS[20]+','+USERS[40]+','+USERS[60]+','+USERS[80]+'\n')
    print("Duration:", time.time()-t0)


