Yifan Wang  
yifanwang0916@outlook.com
    
**** 
 
## Homework1
#### Shannon-Fano in `ShannonFano.hpp`:

    void SF_run(const char* name)
#### Huffman in `Huffman_GS.hpp`:

    void H_run(const string& name)
#### Adaptive Huffman in `Huffman_adp.hpp`:  

    void HA_run(char* name, char* opt)

    opt -> "-c" for conpression
        -> "-d" for decoding
    
**** 

## Homework2
#### BAC in `BAC.hpp`:

    void BAC_run(const char* name, unsigned int A, unsigned int C, int currentState, int MPS, const string& mapping)

    mapping -> "BP" for bit plane
            -> "SF" for Shannon-Fano
            -> "H" for Huffman

#### CABAC in `CABAC.hpp`:

    void CABAC_run(const char* name, int contextbits, const string mapping)

#### Improved CABAC using block mapping in  `CABAC.hpp`:

    void CABAC_A_run(const char* name, int contextbits, int block)

    block -> block size should be divided exactly by image height and width
    
**** 

## Homework3
#### SQ in `SQ.hpp`:

    double SQ_run(vector<string>& train, string& name, const char* save, int bit, int iter)

    train -> list name of training image
    name -> test image
    save -> name of output image
    bit -> quantization bits
    iter -> number of iteration, if large than actual needs would not affect

#### Blocking part in `VQ.hpp`:

    VQ_Block_run(vector<string>& train, int block)

    input image should be 256x256
    
#### Use following to convert quantized image to raw position
    
    void VQ_Deblock_run(string& name, string& save, int block)

#### Standarder VQ use code in sub-floder `stdvd`, use following to install:

    mkdir build
    cd build
    cmake ..
    make
    cd ..

#### TSVQ
Tree structured VQ in sub-floder `tsvq` which include two main function in `tsvq.c` and `tsvqe.c` (one for training, one for testing). Provided code is modied into one single head file due to some bugs from global variables used in original code.
    
**** 

## Homework4
#### DCT, Quantized DCT and Quantized Factor generator in `JPEG.hpp`:

    vector<vector<double>> DCT(const vector<vector<double>>& data)

    vector<vector<int>> QuantizedDCT(const vector<vector<double>>& data)

    data -> 8*8 image blocks

    vector<vector<double>> GenerateQ(int N)

    N -> quality factor

#### JPEG compression use provided code in `jpeg-6b`.  
Install it in `Linux` or `MacOS` use:

    ./configure
    make

Run it use:

    ./cjpeg -quality "N" "input">".jpg"
    -> compress "input" using quality factor "N" and save it as ".jpg"

    ./djpeg ".jpg">".raw"
    -> change ".jpg" to raw data and save it to ".raw"
This raw contains head file as well. If you do want a raw without any header. Use `Mat2Raw` in `Homework3/EE569/Basic.hpp` (openccv needed, setup enviroment and uncommet includes in `core.hpp`)

#### Deblocking use filtering:

    void Deblocking_A_run(string name, int H, int W, int K)

**** 

## Homework5
Run the ffmpeg and x264 code, detail usage can be found in report.
    
**** 

## Homework6
#### PSNR, MSE  
available under `Homework3\EE569\Evaluate\Evaluate.hpp` which can take raw image as input. SSIM in this is not correct.

#### SSIM  
Provided `ssim.c` would not working (compile error), using the matlab code instead. `readraw.m` and `ssim.m`

#### VMAF  
In macOS:
```
brew install libvmaf
brew install pkg-config
```
In `ffmpeg` folder, same as Homework5.
```
./configure --enable-libvmaf --enable-version3
make
```
Run it using 
```
./ffmpeg -pix_fmt yuv420p -s 1920x1080 -i input.yuv -pix_fmt yuv420p -s 1920x1080 -i ori.yuv -lavfi libvmaf='log_path=./vmaf_score.txt:psnr=1:ssim=1' -f null -
```