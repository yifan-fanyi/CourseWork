# 2021.04.09
# @yifan
#
import numpy as np
import cv2
import pickle
import copy
import progressbar
from core.utli import Time

def detect_keypoints(i, isORB=False):
    frame = cv2.imread('./cache/frames/'+str(i)+'.jpg', 0)
    if isORB == True:
        KD = cv2.SIFT_create()
    else:
        KD = cv2.ORB_create(nfeatures=800)
    kp1 = KD.detect(frame, None)
    des1 = []
    #print('   img',i,' Detect', len(kp1),'keypoints')
    if len(kp1) >= 20:
        kp1, des1 = KD.compute(frame, kp1)
    #with open('./cache/keypoint/'+str(i)+'.pkl', 'wb') as f:
    #    pickle.dump({ 'des':des1}, f)
    return des1

def keypoint_matching(des1, des2, isORB=False):
    ct = 0
    if len(des1) < 20 or len(des2) < 20:
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
        matches = flann.knnMatch(np.asarray(des1,np.float32),np.asarray(des2,np.float32),k=2)
        for i,(m,n) in enumerate(matches):
            if m.distance < 0.7*n.distance:
                ct += 1
    return ct

def select_by_KP(n_frames, isSeg, keeprate, history=1):
    isSeg *= 0 
    n_match_list = np.zeros((n_frames))
    p_list = []
    bar = progressbar.ProgressBar(max_value=n_frames)
    buffer = []
    for i in range(n_frames):
        bar.update(i)
        buffer.append(detect_keypoints(i))
        isSeg[i] = 1
        #with open("./cache/keypoint/"+str(i)+'.pkl', 'rb') as f:
        #    des1 = pickle.load(f)['des']
        for j in range(i-1, max(i-1-history, -1), -1):
            #with open("./cache/keypoint/"+str(j)+'.pkl', 'rb') as f:
            #    des2 = pickle.load(f)['des']
            des2 = buffer[j]
            n_match = keypoint_matching(buffer[j], buffer[i])
            if len(buffer[i]) > 0:
                n_match_list[i] = max(n_match/len(buffer[i]), n_match_list[i])
            else:
                n_match_list[i] = 1
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
                              