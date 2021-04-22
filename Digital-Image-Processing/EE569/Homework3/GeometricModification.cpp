//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  GeometricModification.cpp
//  EE569
//
//  Created by Alex on 2019/2/11.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "GeometricModification.hpp"

void SmoothSingleDir(Vec2_ui8& Im2D_1, C_Vec2_i& loc, C_i len2){
    for(int k(-1); k < 2; ++k){
        for(int i(loc[0][0]), ii(0); ii < len2; ++i, ++ii){
            Im2D_1[i][loc[0][1]+k] = (Im2D_1[i][loc[0][1]+k-2]+Im2D_1[i][loc[0][1]+k+2])/2;
        }
        for(int i(loc[0][1]), ii(0); ii < len2; ++i, ++ii){
            Im2D_1[loc[0][0]+k][i] = (Im2D_1[loc[0][0]+k-2][i]+Im2D_1[loc[0][0]+k+2][i])/2;
        }
        for(int i(loc[0][0]), ii(0); ii < len2; ++i, ++ii){
            Im2D_1[i][loc[0][1]+len2-1+k] = (Im2D_1[i][loc[0][1]+len2-3+k]+Im2D_1[i][loc[0][1]+len2+1+k])/2;
        }
        for(int i(loc[0][1]), ii(0); ii < len2; ++i, ++ii){
            Im2D_1[loc[0][0]+len2-1+k][i] = (Im2D_1[loc[0][0]+len2-3+k][i]+Im2D_1[loc[0][0]+len2+1+k][i])/2;
        }
    }
}

void GeoTransform2D(Vec2_ui8& Im2D_1, C_Vec2_ui8& Im2D_2, C_Vec2_i& loc){
    //detect corner point
    Vec2_i LHt_cor = DetectCorner(Im2D_2, 10);
    LHt_cor = SelectCorner(LHt_cor);
    
    //decide angle to rotate
    double angle (((double)LHt_cor[0][1]-(double)LHt_cor[1][1])/(double)((double)LHt_cor[0][0]-(double)LHt_cor[1][0]));
    angle = atan (angle);
    //rotating
    Vec2_ui8 tmp1 = Rotate2D(Im2D_2, -angle);
    int len1 = abs(loc[0][1]-loc[1][1]+1);
    int len2 = sqrt(Sq((double)LHt_cor[0][1]-(double)LHt_cor[1][1])+Sq((double)LHt_cor[0][0]-(double)LHt_cor[1][0]));
    double cosA (cos(-angle)), sinA (sin(-angle));
    int L1 ((Dim1(Im2D_2)>>1)), L2 ((Dim2(Im2D_2)>>1));
    int S((sqrt(Sq(L1)+Sq(L2))));
    for(int i(0); i < 4; i++){
        int ii = cosA*(LHt_cor[i][0]-L1) - sinA*(LHt_cor[i][1]-L2);
        int jj = sinA*(LHt_cor[i][0]-L1) + cosA*(LHt_cor[i][1]-L2);
        LHt_cor[i][0] = ii + S;
        LHt_cor[i][1] = jj + S;
    }
    //scaleing
    double sclar = (double)len1/(double)len2+0.04;
    tmp1 = Scaling(tmp1, sclar);
    
    for(int i(0); i < 4; i++){
        LHt_cor[i][0] = LHt_cor[i][0]*sclar;
        LHt_cor[i][1] = LHt_cor[i][1]*sclar;
    }
    len2 = len2*sclar;
    //put into square vec
    Vec2_ui8 tp init2_ui8(len2, len2, 0);
    for(int i(0),ii(LHt_cor[2][0]); i < len2; ++i, ++ii){
        for(int j(0),jj(LHt_cor[2][1]); j < len2; ++j,++jj){
            tp[i][j] = tmp1[ii][jj];
        }
    }
    //decide wether needs rotate
    double rot = calHoleFillScore(Im2D_1, tp, loc[0][0], loc[0][1], len2);
    if(rot != 0){
        for(int i(0); i < rot; ++i){
            tp = RotateCW90(tp);
        }
    }
    for(int i(loc[0][0]), ii(0); ii < len2; ++i, ++ii){
        for(int j(loc[0][1]), jj(0); jj < len2; ++j, ++jj){
            Im2D_1[i][j] = tp[ii][jj];
        }
    }
    SmoothSingleDir(Im2D_1, loc, len2);
}

Vec3_ui8 GeoTransform(C_Vec3_ui8& Im3D, C_Vec_s& name){
    Vec3_ui8 res (Im3D);
    Vec2_i LH_cor = DetectCornerHole(Im3D[0]);
    int len (Dim1(name));
    Vec3_i cors ({{LH_cor[0],LH_cor[1]},{LH_cor[2],LH_cor[3]},{LH_cor[8],LH_cor[9]}});
    for(int k(0); k < len; ++k){
        Vec3_ui8 tmpI = ReadRaw(str2pchar(name[k]), 1, 256, 256);
        GeoTransform2D(res[0], tmpI[0], cors[k]);
    }
    return res;
}

Vec3_ui8 CallGeoTransform(){
    Vec1_s name = {"lighthouse2.raw", "lighthouse1.raw", "lighthouse3.raw"};
    Vec3_ui8 Im3D = ReadRaw("lighthouse.raw", 1, 512, 512);
    return GeoTransform(Im3D, name);
}

Vec2_ui8 SpatialWarping2D(C_Vec2_ui8& Im2D, C_Vec2_d& coef){
    int H (Dim1(Im2D)), W (Dim2(Im2D));
    int HH (H>>1), WW (W>>1);
    Vec2_ui8 res init2_ui8(Dim1(Im2D),Dim2(Im2D),0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double nx (0), ny (0);
            int ii (i-HH), jj (j-WW);
            if(ii<0 && abs(jj) < abs(ii)){
                nx = coef[0][0] + coef[0][1]*ii + coef[0][2]*jj + coef[0][3]*Sq(ii) + coef[0][4]*ii*jj+coef[0][5]*Sq(jj);
                ny = coef[1][0] + coef[1][1]*ii + coef[1][2]*jj + coef[1][3]*Sq(ii) + coef[1][4]*ii*jj+coef[1][5]*Sq(jj);
            }
            if(jj>0 && abs(ii) < abs(jj)){
                nx = coef[2][0] + coef[2][1]*ii + coef[2][2]*jj + coef[2][3]*Sq(ii) + coef[2][4]*ii*jj+coef[2][5]*Sq(jj);
                ny = coef[3][0] + coef[3][1]*ii + coef[3][2]*jj + coef[3][3]*Sq(ii) + coef[3][4]*ii*jj+coef[3][5]*Sq(jj);
            }
            if(ii>0 && abs(jj) < abs(ii)){
                nx = coef[4][0] + coef[4][1]*ii + coef[4][2]*jj + coef[4][3]*Sq(ii) + coef[4][4]*ii*jj+coef[4][5]*Sq(jj);
                ny = coef[5][0] + coef[5][1]*ii + coef[5][2]*jj + coef[5][3]*Sq(ii) + coef[5][4]*ii*jj+coef[5][5]*Sq(jj);
            }
            if(jj<0 && abs(ii) < abs(jj)){
                nx = coef[6][0] + coef[6][1]*ii + coef[6][2]*jj + coef[6][3]*Sq(ii) + coef[6][4]*ii*jj+coef[6][5]*Sq(jj);
                ny = coef[7][0] + coef[7][1]*ii + coef[7][2]*jj + coef[7][3]*Sq(ii) + coef[7][4]*ii*jj+coef[7][5]*Sq(jj);
            }
            nx = Max(0, Min(nx+HH,H-1));
            ny = Max(0, Min(ny+WW,W-1));
            res[nx][ny] = Im2D[i][j];
        }
    }
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            if(abs(i-HH) == abs(j-WW)){
                res[i][j] = Im2D[i][j];}
        }
    }
    return res;
}

Vec2_ui8 SpatialWarping2D1(C_Vec2_ui8& Im2D, C_Vec2_d& coef){
    int H (Dim1(Im2D)), W (Dim2(Im2D));
    int HH (H>>1), WW (W>>1);
    Vec2_ui8 res init2_ui8(Dim1(Im2D),Dim2(Im2D),0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double nx (0), ny (0);
            int ii (i-HH), jj (j-WW);
            if(ii<0 && abs(jj) < abs(ii)){
                nx = coef[0][0] + coef[0][1]*ii + coef[0][2]*jj + coef[0][3]*Sq(ii) + coef[0][4]*ii*jj+coef[0][5]*Sq(jj);
                ny = coef[1][0] + coef[1][1]*ii + coef[1][2]*jj + coef[1][3]*Sq(ii) + coef[1][4]*ii*jj+coef[1][5]*Sq(jj);
            }
            if(jj>0 && abs(ii) < abs(jj)){
                nx = coef[2][0] + coef[2][1]*ii + coef[2][2]*jj + coef[2][3]*Sq(ii) + coef[2][4]*ii*jj+coef[2][5]*Sq(jj);
                ny = coef[3][0] + coef[3][1]*ii + coef[3][2]*jj + coef[3][3]*Sq(ii) + coef[3][4]*ii*jj+coef[3][5]*Sq(jj);
            }
            if(ii>0 && abs(jj) < abs(ii)){
                nx = coef[4][0] + coef[4][1]*ii + coef[4][2]*jj + coef[4][3]*Sq(ii) + coef[4][4]*ii*jj+coef[4][5]*Sq(jj);
                ny = coef[5][0] + coef[5][1]*ii + coef[5][2]*jj + coef[5][3]*Sq(ii) + coef[5][4]*ii*jj+coef[5][5]*Sq(jj);
            }
            if(jj<0 && abs(ii) < abs(jj)){
                nx = coef[6][0] + coef[6][1]*ii + coef[6][2]*jj + coef[6][3]*Sq(ii) + coef[6][4]*ii*jj+coef[6][5]*Sq(jj);
                ny = coef[7][0] + coef[7][1]*ii + coef[7][2]*jj + coef[7][3]*Sq(ii) + coef[7][4]*ii*jj+coef[7][5]*Sq(jj);
            }
            nx = nx+HH;
            ny = ny+WW;
            if(nx < 0 || ny < 0 || nx >= H || ny >= W){
                res[i][j] = 0;
                continue;
            }

            res[i][j] = Im2D[nx][ny];
        }
    }
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            if(abs(i-HH) == abs(j-WW)){
                res[i][j] = Im2D[i][j];}
        }
    }
    return res;
}

Vec3_ui8 CallSpatialWarping(C_Vec3_ui8& Im3D, C_Vec2_d& coef, C_i opt){
    Vec2_d tmp ({{}});
    if(coef.size() > 1){
        tmp = coef;}
    else{
        if(opt == 0){
            tmp = {
                {0,1,0,0.002,0,-0.002},{0,0,1,0,0,0},
                {0,1,0,0,0,0},{0,0,1,0.002,0,-0.002},
                {0,1,0,-0.002,0,0.002},{0,0,1,0,0,0},
                {0,1,0,0,0,0},{0,0,1,-0.002,0,0.002}};
        }
        else{
            tmp = {
                {0,1,0,-0.0078,0,0.0078},{0,0,1,0,0,0},
                {0,1,0,0,0,0},{0,0,1,-0.0078,0,0.0078},
                {0,1,0,0.0078,0,-0.0078},{0,0,1.0000,0,0,0},
                {0,1,0,0,0,0},{0,0,1,0.0078,0,-0.0078}};
        }
    }
    Vec3_ui8 tmpp (Im3D);
    if(opt < 2){
        tmpp[0] = SpatialWarping2D(Im3D[0], tmp);
    }
    else{
        tmpp[0] = SpatialWarping2D1(Im3D[0], tmp);
    }
    return tmpp;
}

Vec3_ui8 LensDistortionCorrect(C_Vec3_ui8& Im3D){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D));
    int HH (H>>1), WW (W>>1);
    Vec3_ui8 res init3_ui8(K, H, W, 0);
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                double x (i-HH), y (j-WW);
                x = x/600;
                y = y/600;
                double cx, cy;
                NewtonMethod(x, y, cx, cy);
                cx = cx*600;
                cy = cy*600;
                cx += HH;
                cy += WW;
                int xx (Min(H-1, Max(0,cx)));
                int yy (Min(W-1, Max(0,cy)));
                res[k][xx][yy] = Im3D[k][i][j];
            }
        }
    }
    return res;
}

Vec3_ui8 LensDistortionCorrect1(C_Vec3_ui8& Im3D){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D));
    int HH (H>>1), WW (W>>1);
    Vec3_ui8 res init3_ui8(K, H, W, 0);
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                double ii (i-HH), jj (j-WW);
                ii /= 600;
                jj /= 600;
                double r = Sq(ii)+Sq(jj);
                double R = 1-0.3536*r+0.1730*Sq(r);
                double iiR = ii*R;
                double jjR = jj*R;
                double cx = iiR*600+HH;
                double cy = jjR*600+WW;
                int xx (Min(H-1, Max(0,cx)));
                int yy (Min(W-1, Max(0,cy)));
                res[k][i][j] = Im3D[k][xx][yy];
            }
        }
    }
    return res;
}
