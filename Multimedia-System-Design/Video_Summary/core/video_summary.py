# 2021.04.21
# @yifan
#
import numpy as np
import cv2
import os
import pickle
from core.utli import Time
from core.kp_based import KP_based_one_iter
from core.seg_based import Seg_based_one_iter
from core.ssim_based import MS_SSIM_based_one_iter
from core.kp_ssim_based import KP_SSIM_based_one_iter

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
def video_summary(n_frames, keeprate, mode=0):
    print("       <Info> keeprate = %1.4f"%(keeprate))
    isSeg = np.zeros(n_frames).astype('int16')
    rate = 1
    kp_TH = 500
    iou_TH = 0.9
    ssim_TH = 0.4
    while rate > keeprate:
        if mode == 0:
            isSeg, kp_TH = KP_based_one_iter(n_frames, isSeg, keeprate, kp_TH)
        elif mode == 1:
            isSeg, ssim_TH = MS_SSIM_based_one_iter(n_frames, isSeg, keeprate, kp_TH)
        elif mode == 2:
            isSeg, _ = KP_SSIM_based_one_iter(n_frames, isSeg, keeprate, None)
        else:
            isSeg, iou_TH = Seg_based_one_iter(n_frames, isSeg, keeprate, iou_TH)
        rate = np.sum(isSeg == 1) / n_frames
        print("       <Info> %5d/%5d frames retained"%(np.sum(isSeg == 1), n_frames))
        break
        if rate > keeprate:
            print("       <Info> keeprate not satisfied, continue!")
    gen_result(isSeg, False)
    with open('./cache/isSeg.pkl', 'wb') as f:
        pickle.dump(isSeg, f)
    return isSeg

