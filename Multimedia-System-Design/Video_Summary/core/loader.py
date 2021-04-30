# 2021.04.09
# @yifan
# Modified from:
#   https://github.com/kkroening/ffmpeg-python/blob/master/examples/README.md#convert-video-to-numpy-array
#
import numpy as np
import os
import cv2
import ffmpeg
import soundfile as sf
from scipy.io.wavfile import read, write
import sounddevice as sd

def load_stream(name, verbose=False):
    probe = ffmpeg.probe(name)
    video_stream = next((stream for stream in probe['streams'] if stream['codec_type'] == 'video'), None)
    if verbose == True:
        print("------- Start: Video Info -------")
        print("   name:      ", name)
        print("   width:     ", video_stream['width'])
        print("   height:    ", video_stream['height'])
        print("   frame rate:", video_stream['r_frame_rate'])
        print("   duration:  ", video_stream['duration_ts'])
        print("   time_base: ", video_stream['time_base'])
        print("   bit_rate:  ", video_stream['bit_rate'])
        print("-------   End: Video Info -------")
    return video_stream

def video2frameArray(name):
    video_stream = load_stream(name)
    width = int(video_stream['width'])
    height = int(video_stream['height'])
    try:
        out, _ = (
            ffmpeg
            .input(name)
            .output('pipe:', format='rawvideo', pix_fmt='rgb24')
            .run(capture_stdout=True, capture_stderr=True)
        )
    except ffmpeg.Error as e:
        print('stdout:', e.stdout.decode('utf8'))
        print('stderr:', e.stderr.decode('utf8'))
        raise e
    video = (
        np
        .frombuffer(out, np.uint8)
        .reshape([-1, height, width, 3])
    )
    return video, video_stream

def video2Image(Input, Output, useffmpeg=True):
    if useffmpeg == True:
        try:
            (ffmpeg.input(Input)
                .output(Output+'%d.jpg', 
                        video_bitrate='5000k',
                        sws_flags='bilinear',
                        start_number=0)
                .run(capture_stdout=True, capture_stderr=True))
        except ffmpeg.Error as e:
            print('stdout:', e.stdout.decode('utf8'))
            print('stderr:', e.stderr.decode('utf8'))
    else:
        cap = cv2.VideoCapture(Input)
        fps = cap.get(cv2.CAP_PROP_FPS)
        frame_num = 0
        while(True):
            ret, frame = cap.read()
            name = Output + str(frame_num) + '.jpg'
            cv2.imwrite(name, frame)
            currentFrame += 1
        cap.release()
        cv2.destroyAllWindows()

def frameArray2Image(output, images):
    for i in range(len(images)):
        cv2.imwrite(output+str(i)+'.jpg', images[i])

def frame2video(frame_path, output, frame_type='jpg', framerate=25):
    (
        ffmpeg
        .input(frame_path+'*.'+frame_type, pattern_type='glob', framerate=framerate)
        .output(output)
        .run()
    )

def frameArray2video(output, images, framerate=25, vcodec='libx264', images_path=None):
    if images_path is not None:
        images = cv2.imread(images_path+str(0)+'.jpg')
        images = images.reshape(1, images.shape[0], images.shape[1], images.shape[2])
    if not isinstance(images, np.ndarray):
        images = np.asarray(images)
    n,height,width,channels = images.shape
    process = (
        ffmpeg
            .input('pipe:', format='rawvideo', pix_fmt='rgb24', s='{}x{}'.format(width, height))
            .output(output, pix_fmt='yuv420p', vcodec=vcodec, r=framerate)
            .overwrite_output()
            .run_async(pipe_stdin=True)
    )
    if images_path is not None:
        for i in range(500000):
            try:
                x = cv2.imread(images_path+str(i)+'.jpg')
                x.shape
                process.stdin.write(
                        x
                        .astype(np.uint8)
                        .tobytes()
                    )
            except:
                break
    else:
        for frame in images:
            process.stdin.write(
                frame
                    .astype(np.uint8)
                    .tobytes()
            )
    process.stdin.close()
    process.wait()

def load_audio(file):
    a = read(file)
    return a[0], np.array(a[1],dtype='int16')

def load_video(name):
    info = load_stream(name, verbose=True)
    video, video_stream = video2frameArray(name)
    return video, info

def write_audio(name, data, fs):
    write(name, fs, data.astype('int16'))

def raw2jpg(path, name, out_path, H, W):
    for i in range(1, 50000):
        try:
            print(path+name+str(i)+'.rgb')
            fd = open(path+name+str(i)+'.rgb', 'rb')
            cols = H 
            rows = W
            f = np.fromfile(fd, dtype=np.uint8,count=rows*cols*3)
            im = f.reshape((3, rows, cols)) #notice row, column format
            im = np.moveaxis(im, 0, -1)
            tmp = np.zeros_like(im)
            tmp[:, :, 0] = im[:,:,2]
            tmp[:, :, 1] = im[:,:,1]
            tmp[:, :, 2] = im[:,:,0]
            fd.close()
            cv2.imwrite(out_path+str(i)+'.jpg', tmp)
        except:
            return i-1