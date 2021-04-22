//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HalfToning.cpp
//  EE569
//
//  Created by Alex on 2019/1/24.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HalfToning.hpp"

Vec3_ui8 Dithering(C_Vec3_ui8& Im3D){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D));
    Int_RV;
    RV_Uni UniRD (0, 255);
    Vec3_ui8 res (Im3D);
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                res[k][i][j] = Im3D[k][i][j] >= UniRD(IntRand) ? 255 : 0;
            }
        }
    }
    return res;
}

Vec3_ui8 DitheringMatrix(C_Vec3_ui8& Im3D, C_i& Index, C_i& noise){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D));
    Vec3_ui8 res (Im3D);
    Vec2_i IM (genIndexMatrix(Index, true, noise));
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                res[k][i][j] = Im3D[k][i][j] >= (int)toUint(IM[i%Index][j%Index]) ? 255 : 0;
            }
        }
    }
    return res;
}

Vec3_ui8 ErrorDiffusion(C_Vec3_ui8& Im3D, C_i& version, C_i& spread, C_b& color, C_i& threshold){
    int K (Dim1(Im3D));
    Vec3_ui8 Im (Im3D), res;
    if(color == false && K == 3){
        Im = RGB2Gray(Im3D);
        K = 1;
    }
    else if(K == 3){
        Im = toUint(ImgOpposition(Im3D));
    }
    for(int k = 0; k < K; ++k){
        res.push_back(ErrDiffusion2D(Im[k], version, spread, threshold));
    }
    return res;
}

Vec3_ui8 MBVQErrDiff(C_Vec3_ui8& Im3D, C_i& version, C_i& threshold){
    Vec2_d kernel;
    if(version == 0){
        kernel = {{0, 0, 0}, {0, 0, 7}, {3, 5, 1}};
    }
    else if(version == 1){
        kernel = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 7, 5}, {3, 5, 7, 5, 3}, {1, 3, 5, 3, 1}};
    }
    else if(version == 2){
        kernel = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 8, 4}, {2, 4, 8, 4, 2}, {1, 2, 4, 2, 1}};
    }
    else{
        cout<<"MBVQErrDiff: kernel version error"<<endl;
        exit(1);
    }
    return SPSweep3DRefPMBVQ(Im3D, kernel, threshold, true);
}
