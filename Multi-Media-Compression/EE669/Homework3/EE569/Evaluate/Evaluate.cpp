//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  Evaluate.cpp
//  EE569
//
//  Created by Alex on 2019/1/8.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "Evaluate.hpp"

double calSNR(C_c* org, C_c* pro, C_i& K, C_i& H, C_i& W){
    Vec3_ui8 orgImg(ReadRaw(org, K, H, W));
    Vec3_ui8 proImg(ReadRaw(pro, K, H, W));
    return calSNR(orgImg, proImg);
}

double calSNR(C_Vec3_ui8& orgImg, C_Vec3_ui8& proImg){
    int K(Dim1(orgImg));
    int H(Dim2(orgImg));
    int W(Dim3(orgImg));
    double MSE(0), sum(0);
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                MSE += (float)Sq(orgImg[k][i][j]-proImg[k][i][j]);
                sum += (double)Sq(orgImg[k][i][j]);
            }
        }
    }
    double SNR(10*log10(sum/MSE));
    cout<<"SNR: "<<SNR<<endl;
    return SNR;
}

double calPSNR(C_c* org, C_c* pro, C_i& K, C_i& H, C_i& W){
    Vec3_ui8 orgImg(ReadRaw(org, K, H, W));
    Vec3_ui8 proImg(ReadRaw(pro, K, H, W));
    return calPSNR(orgImg, proImg);
}

double calPSNR(C_Vec3_ui8& orgImg, C_Vec3_ui8& proImg){
    int K(Dim1(orgImg));
    int H(Dim2(orgImg));
    int W(Dim3(orgImg));
    double MSE(0);
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                MSE += (float)Sq(orgImg[k][i][j]-proImg[k][i][j]);
            }
        }
    }
    cout<<"MSE: "<<MSE/(H*W)<<endl;
    MSE /= (Sq(255)*K);
    double m(H*W);
    double PSNR(10*log10(m/MSE));
    cout<<"PSNR: "<<PSNR<<endl;
    return PSNR;
}

double calSSIM(C_c* org, C_c* pro, C_i& K, C_i& H, C_i& W){
    Vec3_ui8 orgImg(ReadRaw(org, K, H, W));
    Vec3_ui8 proImg(ReadRaw(pro, K, H, W));
    return calSSIM(orgImg, proImg);
}

double calSSIM(C_Vec3_ui8& orgImg, C_Vec3_ui8& proImg){
    int K(Dim1(orgImg));
    int H(Dim2(orgImg));
    int W(Dim3(orgImg));
    double muX(0), muY(0);
    double sigX2(0), sigY2(0), sigXY2(0);
    double s(H*W*K);
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                muX += orgImg[k][i][j];
                muY += proImg[k][i][j];
            }
        }
    }
    muX /= s;
    muY /= s;
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                sigX2 += Sq(orgImg[k][i][j]-muX);
                sigY2 += Sq(proImg[k][i][j]-muY);
                sigXY2 += (proImg[k][i][j]-muY)*(orgImg[k][i][j]-muX);
            }
        }
    }
    sigX2 /= (s-1);
    sigY2 /= (s-1);
    sigXY2 /= (s-1);
    double sigX(sqrt(sigX2));
    double sigY(sqrt(sigY2));
    double sigXY(sqrt(sigXY2));
    double lXY((2*muX*muY+6.5025)/(Sq(muX)+Sq(muY)+6.5025));
    double cXY((2*sigX2+58.5225)/(sigX2+sigY2+58.5225));
    double sXY((sigXY+29.6125)/(sigX*sigY+29.6125));
    double SSIM(lXY*cXY*sXY);
    cout<<"SSIM: "<<SSIM<<endl;
    return SSIM;
}
