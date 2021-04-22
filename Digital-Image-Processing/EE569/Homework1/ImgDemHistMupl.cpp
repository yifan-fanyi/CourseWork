//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  ImgDemHistMupl.cpp
//  EE569
//
//  Created by Alex on 2019/1/7.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "ImgDemHistMupl.hpp"

Vec3_ui8 BiDemosaicingC(C_Vec3_ui8& Im3D){
    int H (Dim2(Im3D)), W (Dim3(Im3D));
    if(Dim1(Im3D) != 1){
        cout<<"BiDemosaicing: bit per pixel must be 1"<<endl;
        exit(1);
    }
    Vec3_ui8 PIm init3_ui8(3, H, W, 0);
    Vec2_d coreR ({{0.25, 0.5, 0.25},{0.5, 1, 0.5},{0.25, 0.5, 0.25}});
    Vec2_d coreG ({{0, 0.25, 0},{0.25, 1, 0.25},{0, 0.25, 0}});
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            if((i+j)%2 == 0){//G
                PIm[1][i][j] = Im3D[0][i][j];
            }
            else if(i%2 == 0){//R
                PIm[0][i][j] = Im3D[0][i][j];
            }
            else{//B
                PIm[2][i][j] = Im3D[0][i][j];
            }
        }
    }
    PIm[0] = toUint(Conv2D_Ref(PIm[0], coreR, false));
    PIm[1] = toUint(Conv2D_Ref(PIm[1], coreG, false));
    PIm[2] = toUint(Conv2D_Ref(PIm[2], coreR, false));
    /*
    //uncomment this for using zero padding
    PIm[0] = toUint(Conv2D(PIm[0], coreR, false));
    PIm[1] = toUint(Conv2D(PIm[1], coreG, false));
    PIm[2] = toUint(Conv2D(PIm[2], coreR, false));
    */
    return PIm;
}

 Vec3_ui8 MHCDemosaicing(C_Vec3_ui8& Im3D){
     int H (Dim2(Im3D)), W(Dim3(Im3D));
     if(Dim1(Im3D) != 1){
         cout<<"BiDemosaicing: bit per pixel must be 1"<<endl;
         exit(1);
     }
     Vec3_ui8 PIm init3_ui8(3, H, W, 0);
     double t;
     for(int i(2); i < H+2; ++i){
         for(int j(2); j < W+2; ++j){
             if((i+j)%2 == 0){//G
                 if(i%2 == 0){//R
                     t = (5*Im3D[0][i][j]+0.5*(Im3D[0][i-2][j]+Im3D[0][i+2][j])+4*(Im3D[0][i][j-1]+Im3D[0][i][j+1])-Im3D[0][i][j-2]-Im3D[0][i][j+2]-Im3D[0][i-1][j-1]-Im3D[0][i-1][j+1]-Im3D[0][i+1][j-1]-Im3D[0][i+1][j+1])/8;
                     PIm[0][i-2][j-2] = toUint(t);
                     PIm[1][i-2][j-2] = Im3D[0][i][j];
                     t = (5*Im3D[0][i][j]+0.5*(Im3D[0][i-2][j]+Im3D[0][i+2][j])+4*(Im3D[0][i-1][j]+Im3D[0][i+1][j])-Im3D[0][i][j-2]-Im3D[0][i][j+2]-Im3D[0][i-1][j-1]-Im3D[0][i-1][j+1]-Im3D[0][i+1][j-1]-Im3D[0][i+1][j+1])/8;
                     PIm[2][i-2][j-2] = toUint(t);
                 }
                 else{//B
                     t = (5*Im3D[0][i][j]+4*(Im3D[0][i-1][j]+Im3D[0][i+1][j])+0.5*(Im3D[0][i][j-2]+Im3D[0][i][j+2])-Im3D[0][i-2][j]-Im3D[0][i+2][j]-Im3D[0][i-1][j-1]-Im3D[0][i-1][j+1]-Im3D[0][i+1][j-1]-Im3D[0][i+1][j+1])/8;
                     PIm[0][i-2][j-2] = toUint(t);
                     PIm[1][i-2][j-2] = Im3D[0][i][j];
                     t = (5*Im3D[0][i][j]+4*(Im3D[0][i][j-1]+Im3D[0][i][j+1])+0.5*(Im3D[0][i][j-2]+Im3D[0][i][j+2])-Im3D[0][i-2][j]-Im3D[0][i+2][j]-Im3D[0][i-1][j-1]-Im3D[0][i-1][j+1]-Im3D[0][i+1][j-1]-Im3D[0][i+1][j+1])/8;
                     PIm[2][i-2][j-2] = toUint(t);
                 }
             }
             else if(i%2 == 0){//R
                 PIm[0][i-2][j-2] = Im3D[0][i][j];
                 t = (4*Im3D[0][i][j]+2*(Im3D[0][i-1][j]+Im3D[0][i+1][j]+Im3D[0][i][j-1]+Im3D[0][i][j+1])-Im3D[0][i-2][j]-Im3D[0][i+2][j]-Im3D[0][i][j-2]-Im3D[0][i][j+2])/8;
                 PIm[1][i-2][j-2] = toUint(t);
                 t = (6*Im3D[0][i][j]+2*(Im3D[0][i-1][j-1]+Im3D[0][i-1][j+1]+Im3D[0][i+1][j-1]+Im3D[0][i+1][j+1])-1.5*(Im3D[0][i-2][j]+Im3D[0][i+2][j]+Im3D[0][i][j-2]+Im3D[0][i][j+2]))/8;
                 PIm[2][i-2][j-2] = toUint(t);
             }
             else{//B
                 t = (6*Im3D[0][i][j]+2*(Im3D[0][i-1][j-1]+Im3D[0][i-1][j+1]+Im3D[0][i+1][j-1]+Im3D[0][i+1][j+1])-1.5*(Im3D[0][i-2][j]+Im3D[0][i+2][j]+Im3D[0][i][j-2]+Im3D[0][i][j+2]))/8;
                 PIm[0][i-2][j-2] = toUint(t);
                 t = (4*Im3D[0][i][j]+2*(Im3D[0][i-1][j]+Im3D[0][i+1][j]+Im3D[0][i][j-1]+Im3D[0][i][j+1])-Im3D[0][i-2][j]-Im3D[0][i+2][j]-Im3D[0][i][j-2]-Im3D[0][i][j+2])/8;
                 PIm[1][i-2][j-2] = toUint(t);
                 PIm[2][i-2][j-2] = Im3D[0][i][j];
             }
         }
    }
    return PIm;
 }

Vec3_ui8 HistEqMethodA(C_Vec3_ui8& Im3D){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D));
    double S (H * W);
    S = 255/S;
    Vec2_i db (getHistogram(Im3D));
    Vec1_d val (K, 0);
    for(int i(0); i < 256; ++i){
        for(int k(0); k < K; ++k){
            val[k] += db[k][i];
            db[k][i] = val[k]*S;
        }
    }
    save2DHistVec(db, "HistEqMethodA_trans.raw", 1, H, W);
    Vec3_ui8 ImHistEq init3_ui8(K, H, W, 0);
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                ImHistEq[k][i][j] = db[k][Im3D[k][i][j]];
            }
        }
    }
    return ImHistEq;
}

Vec3_ui8 HistEqMethodB(C_Vec3_ui8& Im3D){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D));
    Vec3_ui8 Res;
    for(int k(0); k < K; ++k){
        Res.push_back(HistEqMethodB2D(Im3D[k]));
    }
    Vec2_i CH (getHistogram(Im3D));
    Vec2_i NCH init2_i(K, 256, 0);
    for(int k(0); k < K; ++k){
        double sum (0);
        for(int i(0); i < 256; ++i){
            sum += CH[k][i];
            NCH[k][i] = sum;
        }
    }
    save2DCumHistVec(CH, "HistEqMethodB_cum.raw", 1, H, W);
    return Res;
}
