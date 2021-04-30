# 2021.04.09
# @yifan
#
import numpy as np
import cv2
import pickle
import copy
import progressbar

from core.utli import Time

def detect_keypoints(i):
    frame = cv2.imread('./cache/frames/'+str(i)+'.jpg', 0)
    KD = cv2.SIFT_create()
    #KD = cv2.ORB_create(nfeatures=800)
    kp1 = KD.detect(frame, None)
    des1 = []
    #print('   img',i,' Detect', len(kp1),'keypoints')
    if len(kp1) > 20:
        kp1, des1 = KD.compute(frame, kp1)
    with open('./cache/keypoint/'+str(i)+'.pkl', 'wb') as f:
        pickle.dump({ 'des':des1}, f)

def keypoint_matching(des1, des2, isORB=0):
    ct = 0
    if len(des1) < 20 and len(des2) < 20:
        return 9999
    if isORB == True:
        matcher = cv2.BFMatcher()
        matches = matcher.match(des1, des2)
        ct = len(matches)
    else:
        FLANN_INDEX_KDTREE = 0
        index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
        search_params = dict(checks=50)   # or pass empty dictionary
        flann = cv2.FlannBasedMatcher(index_params, search_params)
        matches = flann.knnMatch(des1,des2,k=2)
    # ratio test as per Lowe's paper
        for i,(m,n) in enumerate(matches):
            if m.distance < 0.7*n.distance:
                ct += 1
    #print(' find',ct,'matching')
    return ct


def select_by_KP_seq(n_frames, isSeg, keeprate, kp_TH, kp_available=False):
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
        for j in range(i-1, max(i-4, -1), -1):
            with open("./cache/keypoint/"+str(j)+'.pkl', 'rb') as f:
                des2 = pickle.load(f)['des']
            n_match = keypoint_matching(des1, des2)
            n_match_list[i] = max(n_match/len(des2[2]), n_match_list[i])
            if n_match < np.max([kp_TH, np.max(n_match_list[i])*0.5]):
                break
    tmp = copy.deepcopy(n_match_list).reshape(-1)
    ct = (1-keeprate) * n_frames
    while ct > 0:
        ii = np.argmax(tmp)
        if isSeg[ii] != 2:
            isSeg[ii] = 2
            ct -= 1
        tmp[ii] = -1
    return isSeg

def select_by_KP(n_frames, isSeg, keeprate, kp_TH):
    stride = (int)(n_frames*keeprate)
    while stride > 0:
        last = -1
        for i in range(0, n_frames, stride):
            if isSeg[i] == 0:
                detect_keypoints(i)
                isSeg[i] = 1
            if last >= 0:
                if isSeg[i] == 1 and isSeg[last] == 1:
                    with open("./cache/keypoint/"+str(i)+'.pkl', 'rb') as f:
                        des1 = pickle.load(f)['des']
                    with open("./cache/keypoint/"+str(last)+'.pkl', 'rb') as f:
                        des2 = pickle.load(f)['des']
                    n_match = keypoint_matching(des1, des2)
                    if n_match > kp_TH:
                        isSeg[last+1:i] = 2
            last = i
        stride = stride // 2
    return isSeg 
                              
def KP_based_one_iter(n_frames, isSeg, keeprate, kp_TH, available=False):
    L = np.sum((isSeg==1).astype('int16'))
    isSeg  = select_by_KP_seq(n_frames, isSeg, keeprate, kp_TH, available)
    if L == np.sum((isSeg==1).astype('int16')):
        kp_TH -= (int)(0.1*kp_TH)
        print("       <Info> KP_TH decay 10 per cent to %4d, currently got %7d frames"%(kp_TH, L))
    return isSeg, kp_TH

