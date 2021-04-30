# 2021.04.09
# @yifan
#
import time
import os
import numpy as np

def Time(method):
    def timed(*args, **kw):
        ts = time.time()
        result = method(*args, **kw)
        te = time.time()
        print("   <RunTime> %s: %4.1f s"%(method.__name__, (te - ts)))
        return result
    return timed

def clean_cache(name, HR):
    os.system("rm -rf ./cache")
    os.system("mkdir ./cache")
    os.system("mkdir ./cache/seg_mask/")
    os.system("mkdir ./cache/keypoint/")
    os.system("mkdir ./cache/result/")
    os.system("mkdir ./cache/result/video_summary/")
    os.system("mkdir ./result/"+name+'/')
    if HR == True:
        os.system("cp -r ./data/"+name+'_hr ./cache/frames')
    else:
        os.system("cp -r ./data/"+name+' ./cache/frames')
        