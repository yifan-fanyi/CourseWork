## Video and Audio Summary
#### last update 2021.05.05
#### @yifan
***
### Video Player --> `player.py`
1. load several frames into a buffer and init the `error=0`;
2. start to play the audio;
3. use `cv2.imshow()` and `cv2.waitkey(1)` show the image;
4. remove the shown frame out of the buffer and try to load new images to buffer; 
5. step 3 & 4 spend `t` and  `t+error` is less than `1/fps`;
    1. use the while loop to control the `1/fps-(t+error)` time delay;
    2. update `error=0`
6. if `t+error` is larger than `1/fps`; 
    1. continue to show next frame;
    2. update error as `t+error-1/fps`;

The step 5 & 6 helps to avoid the error accumulation; If the previous frame use too much time, current and future several frames would show less time to compensate the error;

Input fps and image size affect the player, for large fps and large size, the play may not work well;
```python
player(frames_path,     # frames path
       audio_path,      # audio path
       fps,             # frame rate
       size=None)       # need resize is not None
```
***
## Video Summary Pipeline

<img src="./fig/overview.png" width="600" />

First, perform the video summary based on `SSIM Score` and `Keypoint Score` (introduce later) to get an embedding score (sum them up which gives better objective performance) and select the K frames with the smallest score. Correspondingly we can derive the bool set which indicates whether a frame is retained;

The audio summary is based on the summary result of the video, given the bool index of video summary result;
1. Split the audio into blocks, each block has the same time range each frame;
2. If from frame i to frame i+k, only 1 frame is retained, we check the mean absolute magnitude of the each block;
3. Only retain the one with the largest mean absolute magnitude;

***
### SSIM Score

<img src="./fig/ssim.png" width="600" />

1. Compute the MS-SSIM/SSIM score for each image with its neighbour frames;
    1. original filter size is 11 which is modified to 23 in this case;
    2. in the experiments, calculate the MS-SSIM score with its' 4 previous neighbours;
2. Select the maximum MS-SSIM score (range from 0 to 1) to represent the overlapping with the previous frames;
3. MS-SSIM is costly, SSIM is used;

***
### Keypoint Score

<img src="./fig/keypoint.png" width="600" />

1. For each frame, detect key points and compute the corresponding descriptor;
    1. ORB and SIFT keypoint detectors are used;
    2. ORB with BRIEF descriptor would run faster than SIFT while less accurate;
    3. SIFT can have a matching selection process based on the descriptor distance;
    4. Using SIFT with 16200 detection and matching costs less than 5 minutes
2. Perform keypoint matching using the current frame's descriptor with the previous frames;
    1. To speed up the process, only match the current frame with 4 previous frames;
    2. SIFT with matching point selection is used (better than ORB);
3. For each matching, compute a matching percentage $\frac{\#good-match}{\#keypoint}$;
    1. Normalize to [0,1] better comparison, otherwise some frame with fewer key points may always discard;
    2. Easy embedding with MS-SSIM score;
    3. For each image, we will get 4 matching score, choose the largest one to represent this image;

***
### Segmentation based

1. Generate the segmentation mask for each frame using a ResNet-101 based framework;
    1. The net is pre-trained on Pascal Voc dataset;
2. Calculate the IoU between one frame and its neighbour;
    1. For Pascal Voc there are 21 classes including one unknown and one background;
    2. For computing the IoU, background class are ignored;
3. A maximum IoU score is used to represent the frame;
4. Using the ResNet-101 is pretty time consuming, each frame need 7s on my PC to get a segmentation mask;
    1. `this metric is not included in the pipeline because of time`;

***
### Usage
the main function is in `main.py`
```python 
# put the frame in one folder under ./data
#   rename the image to format start from 0.jpg 
# put the wav file in ./data with the same name as the frame folder's name
# ./result folder saves the result if copy_to_result==True (may need to mkdir it)
# ./cache is the working dir, main will copy data to cache before the pipline
#
main(name='test',            # Sequence frame folder name (under ./data/)
                            # the frame must named from 0.jpg to *.jpg
    mode=2,                 # 0: keypoint based
                            # 1: SSIM based
                            # 2: keypoint & SSIM based
                            # 3: segmentation based
    keeprate=90/540,        # keep rate
    fps=30,                 # framerate
    n_frames=-1,            # number of frames (if -1, it will check automatically)
    clean=True,             # clean the ./cache folder
    israw=False,            # input is raw format
    raw_par={'folder':'test',   # raw format loaction 
             'H':352,           # raw format Height 
             'W':288,           # raw format Width      
             'prefix':'Image'}, # raw format prefix
    copy_to_result=True)    # copy the result from ./cache to ./result

```
*** 
```bash
# run in bash
python3 main.py <name> <mode>
```
### RunTime
`mode=2` (keypoint & ssim) is partially optimized by computating SSIM using multi-process (n_threads can be changed in `./core/kp_ssim_based.py` line17, to disable the optimization, set line14 `fast=False`)
***
#### load for ssim computation (3 threads)

<img src="./fig/load_ssim.png" width="600" />

***

#### load for other part of the main function (cpu is not fully used)

<img src="./fig/load_main.png" width="600" />

***
#### RunTime Log
```bash
alex@Alex Video_Summary % python3 main.py test_video_2 2
************************************************************
 Cleaning cache and setup working dir...
 START
       <Info> keeprate = 0.1667
       <Mode> Select by Keypoints & SSIM!
 Running cmd... : 
      python3 fast_gen.py SSIM 0 5400 0 & python3 fast_gen.py SSIM 5399 10800 1 & python3 fast_gen.py SSIM 10799 16200 2
 99% (5362 of 5401) |################### | Elapsed Time: 0:04:52 ETA:   0:00:02   
   <RunTime> get_ssim: 293.4 s
 99% (5400 of 5401) |################### | Elapsed Time: 0:04:53 ETA:   0:00:00   
   <RunTime> get_ssim: 293.9 s
 99% (5400 of 5401) |################### | Elapsed Time: 0:04:54 ETA:   0:00:00   
   <RunTime> get_ssim: 294.9 s
 Finish running cmd!
   <RunTime> fast_ssim: 305.3 s
 99% (16198 of 16200) |################# | Elapsed Time: 0:03:14 ETA:   0:00:00  
        <Info>  2700/16200 frames retained
   <RunTime> video_summary: 519.1 s
   <RunTime> audio_summary:  1.2 s
 DONE
 Copy the result from ./cache to ./result ...
************************************************************
   <RunTime> main: 546.3 s
```

### [Test Data](https://drive.google.com/drive/folders/1E1w_YoGnwO5owHm_Riny20eczDHQ2Lg2?usp=sharing)
### [Results](https://drive.google.com/drive/folders/1SeBqJZ_YjMDUS4UUrHuvm3p3bIXgPQpU?usp=sharing)