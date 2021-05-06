# 2021.04.09
# @yifan
# fast = True use multiprocess to generate the SSIM score
import numpy as np
import cv2
import os
import pickle
import copy
import progressbar
from core.utli import Time
from core.ssim import SSIM, MS_SSIM
from core.kp_based import detect_keypoints, keypoint_matching

fast = True 

@Time
def fast_ssim(n_frames, n_threads=3):
    a = n_frames // n_threads
    s, e = [0], [a]
    while e[-1] < n_frames:
        s.append(e[-1]-1)
        e.append(min(n_frames, e[-1]+a))
    cmd = ''
    for i in range(len(s)):
        cmd += 'python3 fast_gen.py SSIM '+str(s[i])+' '+str(e[i])+' '+str(i)
        if i < len(s)-1:
            cmd += ' & '
    print(' Running cmd... : \n     ', cmd)
    os.system(cmd)
    while 1: # make sure all process is ended
        try:
            for i in range(len(s)):
                with open('./cache/ssim_'+str(i)+'.pkl', 'rb') as f:
                    d = pickle.load(f)
            break
        except:
            pass
    print(' Finish running cmd!')
    ssim = [[0]]
    for i in range(len(s)):
        with open('./cache/ssim_'+str(i)+'.pkl', 'rb') as f:
            d = pickle.load(f)
        ssim.append(d)
    return np.concatenate(ssim, axis=0)

def select_by_KP_ssim(n_frames, isSeg, keeprate, history=1):
    isSeg *= 0 
    n_match_list = np.zeros((n_frames))
    p_list, buffer = [], []
    bar = progressbar.ProgressBar(max_value=n_frames)
    #with open('./data/ssim.pkl','rb') as f:
    #    ssim_list = pickle.load(f)
    if fast == True:
        ssim_list = fast_ssim(n_frames)
    for i in range(n_frames):
        bar.update(i)
        buffer.append(detect_keypoints(i))
        isSeg[i] = 1
        #with open("./cache/keypoint/"+str(i)+'.pkl', 'rb') as f:
        #    des1 = pickle.load(f)['des']
        if fast == False:
            img1 = cv2.imread('./cache/frames/'+str(i)+'.jpg')
        for j in range(i-1, max(i-1-history, -1), -1):
            if fast == False:
                img2 = cv2.imread('./cache/frames/'+str(j)+'.jpg')
                ssim = SSIM(img1.reshape(1, img1.shape[0], img2.shape[1], -1), img2.reshape(1, img1.shape[0], img2.shape[1], -1))
            else:
                ssim = ssim_list[i]
            #with open("./cache/keypoint/"+str(j)+'.pkl', 'rb') as f:
            #    des2 = pickle.load(f)['des']
            n_match = keypoint_matching(buffer[i], buffer[j])
            if len(buffer[i]) > 0:
                score = ssim + n_match/len(buffer[i])
            else:
                score = ssim + 1
            #print(score, len(des2))
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
                              
