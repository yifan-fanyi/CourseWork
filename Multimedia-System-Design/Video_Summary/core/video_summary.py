# 2021.04.21
# @yifan
#
import numpy as np
import cv2
import os
import pickle
from core.utli import Time
from core.kp_based import select_by_KP
from core.seg_based import select_by_IoU
from core.ssim_based import select_by_ssim
from core.kp_ssim_based import select_by_KP_ssim

def gen_result(idx, return_array=False):
    res, ct = [], 0
    for i in range(idx.shape[0]):
        if idx[i] < 2:
            if return_array == True:
                frame = cv2.imread('./cache/frames/'+str(i)+'.jpg')
                res.append(frame)
            else:
                os.system("cp ./cache/frames/"+str(i)+'.jpg ./cache/result/video_summary/'+str(ct)+'.jpg')
                ct += 1
    return np.array(res)

@Time
def video_summary(n_frames, keeprate, mode=0, available=True):
    print("       <Info> keeprate = %1.4f"%(keeprate))
    isSeg = np.zeros(n_frames).astype('int16')
    if mode == 0:
        print("       <Mode> Select by Keypoints!")
        isSeg = select_by_KP(n_frames, isSeg, keeprate)
    elif mode == 1:
        print("       <Mode> Select by SSIM!")
        isSeg = select_by_ssim(n_frames, isSeg, keeprate)
    elif mode == 2:
        print("       <Mode> Select by Keypoints & SSIM!")
        isSeg = select_by_KP_ssim(n_frames, isSeg, keeprate)
    else:
        print("       <Mode> Select by IoU of Segmentation Mask!")
        isSeg = select_by_IoU(n_frames, isSeg, keeprate, available)
    rate = np.sum(isSeg == 1) / n_frames
    print("       <Info> %5d/%5d frames retained"%(np.sum(isSeg == 1), n_frames))
    gen_result(isSeg, False)
    with open('./cache/isSeg.pkl', 'wb') as f:
        pickle.dump(isSeg, f)
    return isSeg

