# 2021.04.09
# @yifan
#
# if mac os not work point to 
# https://stackoverflow.com/questions/42098126/mac-osx-python-ssl-sslerror-ssl-certificate-verify-failed-certificate-verify
# bash /Applications/Python*/Install\ Certificates.command
#
import cv2
import imageio
from keras_segmentation.pretrained import pspnet_101_voc12
from sklearn.metrics.pairwise import euclidean_distances
from core.utli import *

def color_map(N=256, normalized=False):
    def bitget(byteval, idx):
        return ((byteval & (1 << idx)) != 0)
    dtype = 'float32' if normalized else 'uint8'
    cmap = np.zeros((N, 3), dtype=dtype)
    for i in range(N):
        r = g = b = 0
        c = i
        for j in range(8):
            r = r | (bitget(c, 0) << 7-j)
            g = g | (bitget(c, 1) << 7-j)
            b = b | (bitget(c, 2) << 7-j)
            c = c >> 3
        cmap[i] = np.array([r, g, b])
    cmap = cmap/255 if normalized else cmap
    return cmap

def convert_to_label(x):
    VOC_COLORMAP = color_map(21)
    dist = euclidean_distances(x.reshape(-1, 3), np.array(VOC_COLORMAP))
    return np.argmin(dist, axis=1).reshape(x.shape[0],x.shape[1])

def image_seg(model, frame, i):
    cv2.imwrite("./cache/tmp.jpg", frame)
    out = model.predict_segmentation(
        inp="./cache/tmp.jpg",
        out_fname="./cache/tmp.png"
    )
    cv2.imwrite("./cache/seg_mask/"+str(i)+'.png',convert_to_label(cv2.imread("./cache/tmp.png")))
 
def IoU(pred, target, n_classes=21, ignore=[0]):
    pred = pred.astype('int16')
    target = target.astype('int16')
    I, U = 0., 0.
    for i in range(n_classes):
        if i in ignore:
            continue
        idx1 = pred == i
        idx2 = target == i
        I += np.sum(np.logical_and(idx1, idx2).astype('int16'))
        U += np.sum(np.logical_or(idx1, idx2).astype('int16'))
    if U == 0:
        return 0.
    return I / U
    
def select_by_IoU(n_frames, isSeg, keeprate, iou_TH, available=False, history=1):
    model = pspnet_101_voc12() 
    isSeg *= 0
    iou_list = np.zeros((n_frames, n_frames))
    for i in range(n_frames):
        if available == False:
            frame = cv2.imread('./cache/frames/'+str(i)+'.jpg')
            image_seg(model, frame, i)
        isSeg[i] = 1
        x1 = cv2.imread("./cache/seg_mask/"+str(i)+'.png', 0)
        for j in range(i-1, max(i-1-history, -1), -1):
            if isSeg[j] == 2 or n_match_list[i, j] != 0:
                continue
            x2 = cv2.imread("./cache/seg_mask/"+str(j)+'.png', 0)
            iou = IoU(x1, x2)
            iou_list[i, j] = iou
            if iou > iou_TH:
                isSeg[i] = 2
                break
            if iou < iou_TH*0.5:
                break
    tmp = copy.deepcopy(iou_list).reshape(-1)
    for i in range((int)((1-keeprate) * n_frames)):
        idx = np.argmax(tmp)
        ii = idx // n_frames
        jj = idx % n_frames
        if isSeg[ii] != 2:
            isSeg[ii] = 2
        tmp[idx] = -1
    return isSeg 