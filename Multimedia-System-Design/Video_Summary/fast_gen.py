import numpy as np
import os
import cv2

from core.loader import *
from core.player import *
from core.utli import *
from core.kp_based import *
from core.seg_based import *
from multiprocessing import Process
import sys
import progressbar

@Time
def fast_gen(i, mode):
    i = (int)(i)
    mode = (int)(mode)
    root ='./'
    n = 5
    t = (int)(i)
    n_frame = 30*9*60
    s = i * (n_frame //n)
    e = np.min([(i+1)*(n_frame //n),  n_frame])
    if mode == 1:
        model = pspnet_101_voc12() 
    bar = progressbar.ProgressBar(max_value=e-s)
    for j in range(s, e):
        bar.update(j-s)
        if mode == 0:
            detect_keypoints(i)
        elif mode == 1:
            sseg(j, i, model)

def sseg1(i, idx, model):
    frame = cv2.imread('./cache/frames/'+str(i)+'.jpg')
    cv2.imwrite("./cache/"+str(idx)+".jpg", frame)
    out = model.predict_segmentation(
        inp="./cache/"+str(idx)+".jpg",
        out_fname="./cache/"+str(idx)+".png"
    )
    cv2.imwrite("./cache/seg_mask/"+str(i)+".png", convert_to_label(cv2.imread("./cache/"+str(idx)+".jpg")))

def sseg(i, idx, model):
    #frame = cv2.imread('./cache/frames/'+str(i)+'.jpg')
    #cv2.imwrite("./cache/"+str(idx)+".jpg", frame)
    out = model.predict_segmentation(
        inp='./cache/frames/',
        out_fname="./cache/seg_mask/"
    )
    #cv2.imwrite("./cache/seg_mask/"+str(i)+".png", convert_to_label(cv2.imread("./cache/"+str(idx)+".jpg")))

fast_gen(sys.argv[1], sys.argv[2])