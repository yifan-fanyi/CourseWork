# 2021.05.02
# @yifan
#
import numpy as np
import os
import cv2
import sys
from core.utli import *
from core.loader import *
from core.audio_summary import *
from core.video_summary import *

@Time
def count_frame():
    n_frames = 0
    while 1:
        try:
            x = cv2.imread('./cache/frames/'+str(n_frames)+'.jpg')
            x.shape
            n_frames += 1
        except:
            break
    return n_frames     

@Time
def main(name='test', 
        mode=2,
        keeprate=90/540, 
        fps=30, 
        n_frames=-1, 
        clean=True, 
        israw=False, 
        raw_par={'folder':'test', 'H':352,'W':288, 'prefix':'Image'},
        copy_to_result=True):
    print('************************************************************')
    if israw == True:
        print(" Converting raw to jpg...")
        os.system("mkdir ./data/"+name+'/')
        n_frames = raw2jpg(path='./data/'+folder+'/', name=raw_par['pre_fix'], out_path='./data/'+name+'/', H=raw_par['H'], W=raw_par['W']) 
    available = False
    hr = False
    
    if clean == True:
        print(" Cleaning cache and setup working dir...")
        clean_cache(name, hr, mode)
    if available == True:
        print(" Copy availabled feature to working dir...")
        os.system("cp -r ./data/"+name+"/keypoint ./cache" )
        os.system("cp -r ./data/"+name+"/seg_mask ./cache" )
    if n_frames < 0:
        print('   Counting frames...')
        n_frames = count_frame()
        print('    %7d frames counted'%n_frames)
    print(" START")
    isSeg = video_summary(n_frames, keeprate, mode=mode, available=available)
    fs, audio = load_audio('./data/'+name+'.wav')
    audio_summary(audio, isSeg, fs, fps, return_array=False)
    print(" DONE")
    if copy_to_result == True:
        print(" Copy the result from ./cache to ./result ...")
        os.system("cp -r ./cache/result/ ./result/"+name+'_mode='+str(mode))
    print('************************************************************')

if __name__ == "__main__":
    main(name=sys.argv[1], mode=(int)(sys.argv[2]), n_frames=16200, clean=True)
