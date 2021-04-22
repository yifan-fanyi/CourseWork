//
//  HW1.h
//  CSCI576
//
//  Created by Alex on 2021/2/1.
//

#ifndef HW1_h
#define HW1_h

#include <string>
#include "utli.h"
#include "Image.h"

void SubSampleOneChannel(const double* X, double* subX, const int channel, const int stride, const int H, const int W, const bool widthOnly){
    int pos = 0;
    int Hstride = stride;
    if (widthOnly == TRUE){
        Hstride = 1;
    }
    for (int i = 0; i < H; i+=Hstride){
        for (int j = 0; j < W; j+=stride){
            subX[pos] = X[3*(i*W+j) + channel];
            pos++;
        }
    }
}

double Interploate(double lefttop, double righttop, double leftbottom, double rightbottom, int stride, int i, int j, const bool widthOnly){
    double a11 = lefttop;
    double a21 = righttop - lefttop;
    double a12 = leftbottom - lefttop;
    double a22 = rightbottom + lefttop - leftbottom - righttop;
    int Hstride = stride;
    if (widthOnly == TRUE){
        Hstride = 1;
    }
    double x = (double)(i % Hstride) / (double)Hstride;
    double y = (double)(j % stride) / (double)stride;
    return a11 + a21 * x + a12 * y + a22 * x * y;
}

void UpSampleOneChannel(const double* subX, double* X, const int channel, const int stride, const int H, const int W, const bool widthOnly){
    int Hstride = stride;
    if (widthOnly == TRUE){
        Hstride = 1;
    }
    int H_subX = (int)(H / Hstride);
    int W_subX = (int)(W / stride);
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            int x = (int)(i / Hstride);
            int y = (int)(j / stride);
            double a, b, c, d;
            a = subX[x*W_subX+y];
            b = (x == H_subX-1 or y == W_subX-1) ? a : subX[x*W_subX+y+1];
            c = (x == H_subX-1 or y == W_subX-1) ? a : subX[(x+1)*W_subX+y];
            d = (x == H_subX-1 or y == W_subX-1) ? (b+c)/2 : subX[(x+1)*W_subX+y+1];
            X[3*(i*W+j)+channel] = Interploate(a, b, c, d, stride, i, j, widthOnly);
        }
    }
}

void Quantization(const double* X, double* qX, const int Q, int H, const int W){
    if (Q == 0){
        for (int i = 0; i < H*W*3; i++){
            qX[i] = 0;
        }
        return;
    }
    double Qstep = 256. / (double)Q;
    for (int i = 0; i < H*W*3; i++){
        double val = (double)Clip(X[i]);
        int tmp = (int)(val / Qstep);
        qX[i] = (double)(tmp)*Qstep+Qstep/2;
    }
}

void HW1_Pipline(const string name, const int H, const int W, const int Ystride, const int Ustride, const int Vstride, const int Q, const bool widthOnly){
    // Load images
    MyImage IMG;
    IMG.setImagePath(str2pchar(name));
    IMG.setWidth(W);
    IMG.setHeight(H);
    IMG.ReadImage();
    double* YUV = new double[H*W*3];
    
    // RGB2YUV
    RGB2YUV(IMG.getImageData(), YUV, H, W);
    double* upYUV = new double[H*W*3];
    int HYstride = Ystride, HVstride = Vstride, HUstride = Ustride;
    if (widthOnly == TRUE){
        HYstride = 1;
        HVstride = 1;
        HUstride = 1;
    }
    // sub-sampling
    double* subY = new double[(H/HYstride) * (W/Ystride)];
    SubSampleOneChannel(YUV, subY, 0, Ystride, H, W, widthOnly);
    double* subU = new double[(H/HUstride) * (W/Ustride)];
    SubSampleOneChannel(YUV, subU, 1, Ustride, H, W, widthOnly);
    double* subV = new double[(H/HVstride) * (W/Vstride)];
    SubSampleOneChannel(YUV, subV, 2, Vstride, H, W, widthOnly);
    delete[] YUV;
    
    // up-sampling
    UpSampleOneChannel(subY, upYUV, 0, Ystride, H, W, widthOnly);
    UpSampleOneChannel(subU, upYUV, 1, Ustride, H, W, widthOnly);
    UpSampleOneChannel(subV, upYUV, 2, Vstride, H, W, widthOnly);
    delete[] subY;
    delete[] subU;
    delete[] subV;
    
    // YUV2RGB
    double* RGB = new double[H*W*3];
    YUV2RGB(upYUV, RGB, H, W);
    delete[] upYUV;
    
    // quantization
    double* qRGB = new double[H*W*3];
    Quantization(RGB, qRGB, Q, H, W);
    delete[] RGB;
    
    // write
    long int pos = name.find(".");
    string save = name.substr (0,pos) + "_" + \
                   to_string(Ystride) + "_" + \
                   to_string(Ustride) + "_" + \
                   to_string(Vstride) + "_" + \
                   to_string(Q) + ".raw";
    save_RGB(qRGB, save, H, W);
}

void run_HW1(int argc, const char * argv[], const int H, const int W){
    if (argc < 6){
        printf("\nERROR! Too less arguments! \n");
        printf("   HW1 Usage: ./main <image> <Ystride> <Ustride> <Vstride> <Q>\n");
        exit(1);
    }
    bool widthOnly = TRUE;
    if (argc == 7){
        if (str2int(argv[6]) == 1){
            widthOnly = TRUE;
        }
        else{
            widthOnly = FALSE;
        }
    }
    HW1_Pipline(argv[1], H, W, str2int(argv[2]), str2int(argv[3]), str2int(argv[4]), str2int(argv[5]), widthOnly);
}
#endif /* HW1_h */
