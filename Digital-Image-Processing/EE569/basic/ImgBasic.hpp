//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  ImgBasic.hpp
//  EE569
//
//  Created by Alex on 2019/1/15.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef ImgBasic_hpp
#define ImgBasic_hpp

#include "Basic.hpp"

using namespace std;

//count for distribution of pixel value
template<class T>
Vec2_i getHistogram(const Vec3_T& Img3D){
    Vec2_i db init2_i(Dim1(Img3D), 256, 0);
    for(int k(0); k < Dim1(Img3D); ++k){
        for(int i(0); i < Dim2(Img3D); ++i){
            for(int j(0); j < Dim3(Img3D); ++j){
                ++db[k][(int)Img3D[k][i][j]];
            }
        }
    }
    return db;
}

//Otsu method
template<class T>
double OtsuT(const Vec2_T& Img2D){
    double thresHold (0);
    Vec1_d hist (256, 0);
    double sumHist(Dim1(Img2D)*Dim2(Img2D));
    double g (0), maxg (0), u0 (0), u1 (0), w0 (0);
    double w1 (0), N0 (0), N1 (0);
    for(int i(0); i < Dim1(Img2D); ++i){
        for(int j(0); j < Dim2(Img2D); ++j){
            ++hist[Img2D[i][j]];
        }
    }
    for(int i(0); i < 256; ++i){
        for(int j(0); j < i; ++j){N0 += hist[j];}
        w0 = N0 / sumHist;
        for(int j(0); j < i; ++j){u0 += hist[j] * j;}
        if(N0 == 0){u0 = 0;}
        else{u0 /= N0;}
        N1 = sumHist - N0;
        w1 = 1 - w0;
        for(int j(i); j < 256; ++j){u1 += hist[j] * j;}
        if(N1 == 0){u1 = 0;}
        else{
            u1 /= N1;}
        g = w0 * w1 * Sq(u0 - u1);
        if(g > maxg){
            maxg = g;
            thresHold = i;
        }
    }
    return thresHold;
}

//th = -1: use Otsu threshold
//th >= 0: use th as threshold
template<class T>
Vec3_ui8 ImgBinary(const Vec3_T& Img3D, int threshold){
    Vec3_ui8 res (Img3D);
    for(int k(0); k < Dim1(Img3D); ++k){
        if(threshold == -1){threshold = OtsuT(Img3D[k]);}
        for(int i(0); i < Dim2(Img3D); ++i){
            for(int j(0); j < Dim3(Img3D); ++j){
                res[k][i][j] = Img3D[k][i][j] >= threshold ? 255 : 0;
            }
        }
    }
    return res;
}

template<class T>
Vec3_T RGB2Gray(const Vec3_T& Img3D){
    Vec3_T res init3_ui8(1, Dim2(Img3D), Dim3(Img3D), 0);
    for(int i(0); i < Dim2(Img3D); ++i){
        for(int j(0); j < Dim3(Img3D); ++j){
            double t(0.299*Img3D[0][i][j]+0.587*Img3D[1][i][j]+0.114*Img3D[2][i][j]);
            res[0][i][j] = (T)t;
        }
    }
    return res;
}

//RGB2CMY
//or can be used as image opposite
template<class T>
Vec3_d ImgOpposition(const Vec3_T& Img3D){
    Vec3_d res;
    Vec2_T t init2_T(Dim2(Img3D), Dim3(Img3D), 255);
    for(int k(0); k < Dim1(Img3D); ++k){
        res.push_back(Add2D(t, Img3D[k], -1));
    }
    return res;
}

//RGB2Lab
template<class T>
Vec3_d RGB2Lab(const Vec3_T& RGB){
    int H (Dim2(RGB));
    int W (Dim3(RGB));
    Vec3_d Lab init3_d(3, H, W, 0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double r (RGB[0][i][j]);
            double g (RGB[1][i][j]);
            double b (RGB[2][i][j]);
            Vec1_d xyz (3, 0);
            xyz[0] = (r*0.4124 + g*0.3576 + b*0.1805)/95.047;
            xyz[1] = (r*0.2126 + g*0.7152 + b*0.0722)/100;
            xyz[2] = (r*0.0193 + g*0.1192 + b*0.9505)/108.883;
            for(int k(0); i < 3; ++i){
                if(xyz[k] > 0.008856){
                    xyz[k] = pow(xyz[k], 0.3333333333333333);
                }
                else{
                    xyz[k] = 7.787*xyz[k] + 16.0/116.0;
                }
            }
            Lab[0][i][j] = (116*xyz[1] ) - 16;
            Lab[1][i][j] = 500*(xyz[0] - xyz[1]);
            Lab[2][i][j] = 200*(xyz[1] - xyz[2]);
        }
    }
    return Lab;
}

//rotate by angle
//angle: double -pi to pi
template<class T>
Vec2_T Interpolation(const Vec2_T& Im2D){
    int H (Dim1(Im2D)), W (Dim2(Im2D));
    Vec2_T res init2_T(H, W, 0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            if(Im2D[i][j] == 0){
                int ct(0);
                double sum(0);
                for(int ii(max(i-1, 0)); ii < min(i+2, H); ++ii){
                    for(int jj(max(j-1, 0)); jj < min(j+2, W); ++jj){
                        if(Im2D[ii][jj] != 0){
                            sum+=Im2D[ii][jj];
                            ++ct;}
                    }
                }
                res[i][j] = (T)sum/ct;
            }
            else{
                res[i][j] = Im2D[i][j];
            }
        }
    }
    return res;
}

//using average
template<class T>
Vec2_T Rotate2D(const Vec2_T& Img2D, C_d& angle){
    int H (Dim1(Img2D)), W (Dim2(Img2D));
    int S((sqrt(Sq(H)+Sq(W))));
    Vec2_T res init2_T(S, S, 0);
    S = S>>1;
    int HH((H>>1));
    int WW((W>>1));
    double cosA(cos(angle));
    double sinA(sin(angle));
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            int ii = cosA*(i-HH) - sinA*(j-WW);
            int jj = sinA*(i-HH) + cosA*(j-WW);
            res[ii+S][jj+S] = Img2D[i][j];
        }
    }
    res = Interpolation(res);
    return res;
}

//Scaling
//s < 1, random choose a pixel
//s > 1 interpolation
template<class T>
Vec2_T Scaling(const Vec2_T& Img2D, C_d& s){
    int H (Dim1(Img2D)), W (Dim2(Img2D));
    int Hs (H*s), Ws (W*s);
    Vec2_T res init2_T(Hs, Ws, 0);
    for(int i(0); i < H; ++i){
        int k = i*s;
        for(int j(0); j < W; ++j){
            res[k][(int)j*s] = Img2D[i][j];
        }
    }
    if(s>1){
        return Interpolation(res);
    }
    return res;
}

#endif /* ImgBasic_hpp */

