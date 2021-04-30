# 2021.04.09
# @yifan
# First read several frames into the buffer, then start playing using the cv2.inshow, 
#   play the audio at the same time using simpleaudio
# The time delay for each frame is controled by while loop
# a frame would show at least 1ms by cv2.waitKey(1)
# An error diffusion method is used, if curent frame shows more time than 1/fps, 
#  the error would be recorded and the next several frames will be shown in a shorter 
#  time to make up the time comsumption. In that case, no accumulated error would happen;
# Input fps and fame size affect the player
#
import numpy as np
import cv2
import time
from matplotlib import pyplot as plt
import matplotlib
matplotlib.use('TkAgg')
import simpleaudio 

def load_one_img(frames_path, i, size):
    x = cv2.imread(frames_path+str(i)+'.jpg')
    if size is not None:
        x = cv2.resize(x, size)
    return x

def player(frames_path, audio_path, fps, size=None):
    fps = 1. / fps                               # show time for each frame
    e = 0                                        # accumulated error
    buff, buff_size = [], 20                     # buffer for storing the image to be shown
    for i in range(buff_size):                   # pre-load some images
        buff.append(load_one_img(frames_path, i, size))
    if audio_path is not None:
        wave_obj = simpleaudio.WaveObject.from_wave_file(audio_path)   # play audio
        wave_obj.play()
    t1 = time.time()
    for i in range(buff_size, 50000):
        try:
            cv2.imshow(frames_path, buff[0])
            start = time.time()
            buff.pop(0)                                         # remove already shown image
            try:
                x = load_one_img(frames_path, i, size)          # try to keep loading images to buffer
                x.shape
                buff.append(x)
            except:
                pass
            cv2.waitKey(1)                                     # imshow
            diff = time.time() - start + e                     # time already used, e helps to correct, if last frame use too much time, this frame use less
            while diff < fps:                                  # time delay
                diff = time.time() - start + e
            e = diff - fps                                     # update e
            #print(time.time() - start)
        except:
            break
    cv2.destroyAllWindows()


if __name__ == "__main__":
    import sys
    #player('/Users/alex/Desktop/data/test/', "/Users/alex/Desktop/data/test.wav", 30)
    fps = 30
    if len(sys.argv) > 3:
        fps = (int)(sys.argv[3])
    player(sys.argv[1], sys.argv[2], fps)