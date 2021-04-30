# 2021.04.21
# @yifan
#
import os
import numpy as np
import soundfile as sf
from core.utli import Time

def remove_silent(audio, silence_threshold=1/5, block_size=(int)(0.03*48000)):
    max_vol = np.mean(np.abs(audio))
    res = []
    for i in range(0, len(audio), block_size):
        if np.mean(np.abs(audio[i:i+block_size])) < max_vol * silence_threshold:
            audio[i:i+block_size] *= 0
    return audio

def Select(audio, sample_p_frame):
    eng,tt = [],[]
    tmp = audio
    sample_p_frame = (int)(sample_p_frame)
    for i in range(0, len(tmp), (int)(sample_p_frame)//10):
        eng.append(np.mean(np.abs(tmp[i:i+sample_p_frame//10])))
        tt.append(tmp[i:i+sample_p_frame//10])
    idx = np.argsort(eng)[-10:]
    idx.sort()
    res = []
    for i in idx:
        res.append(tt[i])
    return np.concatenate(res, axis=0)

def video_based_audio_removal(audio, idx):
    sample_p_frame = len(audio) / (len(idx))
    last = 0
    res = []
    for i in range(1, len(idx)):
        if idx[i] == 1:
            res.append(Select(audio[(int)(last*sample_p_frame):(int)(i*sample_p_frame)], sample_p_frame))
            last = i
    return np.concatenate(res, axis=0)

@Time 
def audio_summary(audio, idx, fs, return_array=False):
    audio = video_based_audio_removal(audio, idx)
    if return_array == True:
        return np.concatenate(res, axis=0)
    else:
        sf.write('./cache/result/audio_summary.wav', audio, fs, 'PCM_16')

# not used
def find_SV(block, audio, res=100):
    X = view_as_windows(audio, (len(block), 1), (res, 1))
    X = np.moveaxis(X, 1, -1)
    X = X.reshape(X.shape[0], X.shape[1], -1)
    err = np.abs(X - block)
    err = np.mean(err.reshape(err.shape[0], -1), axis=1)
    idx = np.argmin(err)
    return err[idx]

def remove_duplicate(audio, err_threshold, block_size=(int)(0.03*48000)):
    res = []
    for i in range(0, len(audio)-100*block_size, block_size):
        e = find_SV(audio[i:i+block_size],audio[i+block_size:i+99*block_size], res=100)
        print(e)
        if e < err_threshold:
            continue
        else:
            res.append(audio[i:i+block_size])
    return np.concatenate(res,axis=0)

def process_block(x):
    x = block_reduce(x, (2, 1), np.mean)
    return x #idct(x, axis=0, norm='ortho')

def audio_summary_old(audio, sec_retain, fs=16000):
    res = []
    seg_size = 2000
    for i in range(0, len(audio), seg_size):
        res.append(process_block(audio[i:i+seg_size]))
    return np.concatenate(res,axis=0)