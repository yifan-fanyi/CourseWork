# Alex
# 2019.04.11
# edge_demo.py
# demo on extracting edges used in new loss function
# EE599 project

import numpy as np
import cv2
from scipy import signal

img = cv2.imread('2007_000129_p.png',0)
img = np.float32(img)
img = img.reshape(500,334)
tmp = signal.convolve2d(img, np.ones((3,3)), mode='same')

img = 9*img - tmp
img[img == 0] = 0
img[img != 0] = 1
y_p = img

img = cv2.imread('2007_000129.png',0)
img = np.float32(img)
img = img.reshape(500,334)

img[img < 220] = 0
img[img > 220] = 1
img = 255*(1-img)*(y_p)
cv2.imwrite('edge.jpg',img)
    
    
    
  
