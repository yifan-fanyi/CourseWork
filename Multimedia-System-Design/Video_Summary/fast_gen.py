# generate the keypoint or segmask in parallel
#
'''
python3 fast_gen.py Key 0 0 & \
python3 fast_gen.py Key 0 1 & \
python3 fast_gen.py Key 0 2 & \
python3 fast_gen.py Key 0 3 & \
python3 fast_gen.py Key 0 4 & \
python3 fast_gen.py Key 0 5 & \
python3 fast_gen.py Key 0 6 & \
python3 fast_gen.py Key 0 7 
'''
import numpy as np
import os
import cv2
import sys
import progressbar
from core.loader import *
from core.utli import *
from core.kp_based import *
from core.seg_based import *
from multiprocessing import Process
import pickle
import copy
from core.ssim import SSIM, MS_SSIM


name = 'concert_2'
def detect_keypoints(i):
    frame = cv2.imread('./data/'+name+'/'+str(i)+'.jpg', 0)
    KD = cv2.SIFT_create()
    #KD = cv2.ORB_create(nfeatures=800)
    kp1 = KD.detect(frame, None)
    des1 = []
    #print('   img',i,' Detect', len(kp1),'keypoints')
    if len(kp1) > 20:
        kp1, des1 = KD.compute(frame, kp1)
    with open('./data/'+name+'/keypoint/'+str(i)+'.pkl', 'wb') as f:
        pickle.dump({ 'des':des1}, f)

@Time
def fast_gen(i, mode):
    try:
        os.system('mkdir ./data/'+name+'/keypoint')
        os.system('mkdir ./data/'+name+'/seg_mask')
    except:
        pass
    i = (int)(i)
    mode = (int)(mode)
    root ='./'
    n = 8
    t = (int)(i)
    n_frame = 30*9*60
    s = i * (n_frame //n)
    e = np.min([(i+1)*(n_frame //n),  n_frame])
    if mode == 1:
        model = pspnet_101_voc12() 
    bar = progressbar.ProgressBar(max_value=e-s)
    print('start from ',s, 'end:' ,e)
    for j in range(s, e):
        bar.update(j-s)
        if mode == 0:
            detect_keypoints(j)
        elif mode == 1:
            sseg(j, i, model)

def sseg(i, idx, model):
    frame = cv2.imread('./data/'+name+'/'+str(i)+'.jpg')
    cv2.imwrite("./cache/"+str(idx)+".jpg", frame)
    out = model.predict_segmentation(
        inp="./cache/"+str(idx)+".jpg",
        out_fname="./cache/"+str(idx)+".png"
    )
    cv2.imwrite("./data/"+name+"/seg_mask/"+str(i)+".png", convert_to_label(cv2.imread("./cache/"+str(idx)+".jpg")))

@Time
def get_ssim(s, e, name):
    ssim = []
    bar = progressbar.ProgressBar(max_value=e-s)
    for i in range(s+1, e):
        img1 = cv2.imread('./cache/frames/'+str(i)+'.jpg')
        img2 = cv2.imread('./cache/frames/'+str(i-1)+'.jpg')
        img1 = img1.reshape(1, img1.shape[0], img1.shape[1], -1)
        img2 = img2.reshape(1, img2.shape[0], img2.shape[1], -1)
        ss = SSIM(img1, img2)
        ssim.append(ss)
        bar.update(i-s)
    with open('./cache/ssim_'+name+'.pkl', 'wb') as f:
        pickle.dump(ssim, f)

if __name__ == "__main__":
    if sys.argv[1] == 'Key':
        fast_gen(sys.argv[3], sys.argv[2])
    else:
        get_ssim((int)(sys.argv[2]), (int)(sys.argv[3]), sys.argv[4])