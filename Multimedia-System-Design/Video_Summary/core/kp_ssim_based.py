# 2021.04.09
# @yifan
#
import numpy as np
import cv2
import pickle
import copy
import progressbar

from core.utli import Time
from core.ssim import MS_SSIM
from core.kp_based import *

def select_by_KP_ssim_seq(n_frames, isSeg, keeprate, kp_TH, kp_available=False):
    isSeg *= 0 
    n_match_list = np.zeros((n_frames))
    p_list = []
    bar = progressbar.ProgressBar(max_value=n_frames)
    for i in range(n_frames):
        bar.update(i)
        if kp_available == False:
            detect_keypoints(i)
        isSeg[i] = 1
        with open("./cache/keypoint/"+str(i)+'.pkl', 'rb') as f:
            des1 = pickle.load(f)['des']
        img1 = cv2.imread('./cache/frames/'+str(i)+'.jpg')
        for j in range(i-1, max(i-4, -1), -1):
            img2 = cv2.imread('./cache/frames/'+str(j)+'.jpg')
            ssim = MS_SSIM(img1.reshape(1, img1.shape[0], img2.shape[1], -1), img2.reshape(1, img1.shape[0], img2.shape[1], -1))
            with open("./cache/keypoint/"+str(j)+'.pkl', 'rb') as f:
                des2 = pickle.load(f)['des']
            n_match = keypoint_matching(des1, des2)
            score = ssim + n_match/len(des2[2])
            n_match_list[i] = max(score, n_match_list[i])
    tmp = copy.deepcopy(n_match_list).reshape(-1)
    ct = (1-keeprate) * n_frames
    while ct > 0:
        ii = np.argmax(tmp)
        if isSeg[ii] != 2:
            isSeg[ii] = 2
            ct -= 1
        tmp[ii] = -1
    return isSeg
                              
def KP_SSIM_based_one_iter(n_frames, isSeg, keeprate, kp_TH, available=False):
    L = np.sum((isSeg==1).astype('int16'))
    isSeg  = select_by_KP_ssim_seq(n_frames, isSeg, keeprate, kp_TH, available)
    if L == np.sum((isSeg==1).astype('int16')):
        kp_TH -= (int)(0.1*kp_TH)
        print("       <Info> KP_TH decay 10 per cent to %4d, currently got %7d frames"%(kp_TH, L))
    return isSeg, kp_TH

