# 2021.04.21
# @yifan
#
import numpy as np
import cv2
import progressbar
import copy
import pickle
from core.utli import *
from core.ssim import SSIM, MS_SSIM

def select_by_ssim(n_frames, isSeg, keeprate, kp_available=False, history=2):
    isSeg *= 0 
    n_match_list = np.zeros((n_frames))
    p_list = []
    bar = progressbar.ProgressBar(max_value=n_frames)
    for i in range(n_frames):
        bar.update(i)
        isSeg[i] = 1
        img1 = cv2.imread('./cache/frames/'+str(i)+'.jpg')
        for j in range(i-1, max(i-1-history, -1), -1):
            img2 = cv2.imread('./cache/frames/'+str(j)+'.jpg')
            ssim = SSIM(img1.reshape(1, img1.shape[0], img2.shape[1], -1), img2.reshape(1, img1.shape[0], img2.shape[1], -1))
            n_match_list[i] = max(ssim, n_match_list[i])
    tmp = copy.deepcopy(n_match_list).reshape(-1)
    ct = (1-keeprate) * n_frames
    while ct > 0:
        ii = np.argmax(tmp)
        if isSeg[ii] != 2:
            isSeg[ii] = 2
            ct -= 1
        tmp[ii] = -1
    return isSeg
