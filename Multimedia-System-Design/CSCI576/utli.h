//
//  utli.h
//  CSCI576
//
//  Created by Alex on 2021/2/2.
//

#ifndef utli_h
#define utli_h
#include <string>
#include <sstream>
#include <iostream>
#include "Image.h"

using namespace std;
int str2int(const string s){
    istringstream is(s);
    int res;
    is>>res;
    return res;
}

double str2double(const string s){
    return stod(s);
}

const char* str2pchar(const string& s){
    const char *p = s.c_str();
    return p;
}

int Clip(double val){
    val = val > 255 ? 255 : val;
    val = val < 0   ? 0   : val;
    return val;
}

char toChar(double val){
    int tmp = (int)(val+0.5);
    tmp = Clip(tmp);
    return (char)tmp;
}

char toChar(int val){
    int tmp = (int)val;
    tmp = Clip(tmp);
    return (char)tmp;
}

int Clip(int val){
    val = val > 255 ? 255 : val;
    val = val < 0   ? 0   : val;
    return val;
}

void ClipIMG(const double* X, char* res, int H, int W){
    for (int i = 0; i < H*W*3; i++){
        res[i] = Clip(X[i]);
    }
}

double Max(const double a, const double b){
    return a > b ? a : b;
}

double Min(const double a, const double b){
    return a > b ? b : a;
}

double Mod(const double a, const double b){
    double t = a;
    if (t < 0){
        while (t < 0){
            t += b;
        }
    }
    else{
        while (t > b){
            t -= b;
        }
    }
    return t;
}

double Abs(double a){
    return a > 0 ? a : -a;
}

void IMG_todouble(const char* RGB, double* dRGB, const int H, const int W){
    for (int i = 0; i < H * W; i++){
        double R = (uint8_t)(RGB[3*i  ]);
        double G = (uint8_t)(RGB[3*i+1]);
        double B = (uint8_t)(RGB[3*i+2]);
        dRGB[3*i]   = R;
        dRGB[3*i+1] = G;
        dRGB[3*i+2] = B;
    }
}

// rgb packed
void RGB2YUV(const char* RGB, double* YUV, const int H, const int W){
    for (int i = 0; i < H * W; i++){
        double R = (uint8_t)(RGB[3*i  ]);
        double G = (uint8_t)(RGB[3*i+1]);
        double B = (uint8_t)(RGB[3*i+2]);
        YUV[3*i]   = 0.299    * R + 0.587   * G + 0.114   * B;
        YUV[3*i+1] = 0.596    * R - 0.274   * G - 0.322   * B;
        YUV[3*i+2] = 0.211    * R - 0.523   * G + 0.312   * B;
    }
}

void YUV2RGB(const double* YUV, double* RGB, const int H, const int W){
    for (int i = 0; i < H * W; i++){
        double Y = double(YUV[3*i  ]);
        double U = double(YUV[3*i+1]);
        double V = double(YUV[3*i+2]);
        RGB[3*i]   = 1 * Y + 0.956   * U + 0.621   * V;
        RGB[3*i+1] = 1 * Y - 0.272   * U - 0.647   * V;
        RGB[3*i+2] = 1 * Y - 1.106   * U + 1.703   * V;
    }
}

void split_channel(const double* img, double* R, double* G, double* B, const int H, const int W){
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            R[i*W+j] = img[3*(i*W+j)];
            G[i*W+j] = img[3*(i*W+j)+1];
            B[i*W+j] = img[3*(i*W+j)+2];
        }
    }
}

void combine_channel(double* img, const double* R, const double* G, const double* B, const int H, const int W){
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            img[3*(i*W+j)] = R[i*W+j];
            img[3*(i*W+j)+1] = G[i*W+j];
            img[3*(i*W+j)+2] = B[i*W+j];
        }
    }
}

void save_RGB(const double* RGB, const string path, const int H, const int W){
    char* cRGB = new char[H*W*3];
    ClipIMG(RGB, cRGB, H, W);
    MyImage result;
    result.setImagePath(str2pchar(path));
    result.setWidth(W);
    result.setHeight(H);
    result.setImageData(cRGB);
    result.WriteImage();
    cout<<"Save image as: "<<path<<endl;
}
void save_YUV(const double* YUV, const string path, const int H, const int W){
    double* RGB = new double[H*W*3];
    YUV2RGB(YUV, RGB, H, W);
    save_RGB(RGB, path, H, W);
}

#endif /* utli_h */
