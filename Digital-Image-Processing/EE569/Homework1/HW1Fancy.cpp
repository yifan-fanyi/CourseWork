//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW1Fancy.cpp
//  EE569
//
//  Created by Alex on 2019/1/25.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HW1Fancy.hpp"

Vec3_ui8 HW1Fancy1(){
    Vec3_ui8 Im3D = ReadRaw("data/rose_color_noise.raw", 3, 256, 256);
    Vec3_ui8 t0 (Seg(Im3D, 1.7, 3, 3));
    Vec3_ui8 t1 (UniformFilter(Im3D, 3));
    t1 = NLMeansFilter(t1, 19.3, 15, 5);
    Vec3_ui8 t2 (GaussianFilter(t1, 5, 1));
    Vec3_ui8 t3 (ImgBinary(t0, 255));
    Vec3_ui8 t4 (Im3D);
    WriteRaw("data/Seg.raw", t3);
    for(int i(0); i < 256; ++i){
        for(int j(0); j < 256; ++j){
            for(int k(0); k < 3; ++k){
                t4[k][i][j] = t3[1][i][j] != 0 ? t1[k][i][j]:t2[k][i][j];
            }
        }
    }
    WriteRaw("data/Fancy1_result.raw", t4);
    calPSNR("data/rose_color.raw", "data/Fancy1_result.raw", 3, 256, 256);
    calSSIM("data/rose_color.raw", "data/Fancy1_result.raw", 3, 256, 256);
    return t4;
}

Vec3_ui8 Seg(C_Vec3_ui8& Im3D, C_d& sigma, C_i& search_window, C_i& compare_window){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D));
    int n(search_window>>1), nn (compare_window>>1);
    double sigma2 (Sq(sigma));
    Vec3_ui8 Res (Im3D);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            Vec1_d sum (3,0);
            Vec1_d piV (K, 0);
            Vec1_d avgPo (K, 0);
            int ct (0);
            for(int iii(max(i-nn, 0)); iii < min(i+nn+1,H); ++iii){
                for(int jjj(max(j-nn, 0)); jjj < min(j+nn+1,W); ++jjj){
                    for(int k(0); k < K; ++k){
                        avgPo[k] += Im3D[k][iii][jjj];
                    }
                    ++ct;
                }
            }
            for(int k(0); k < K; ++k){
                avgPo[k] /= ct;
            }
            for(int ii(max(i-n, 0)); ii < min(i+n+1,H); ++ii){
                for(int jj(max(j-n, 0)); jj < min(j+n+1,W); ++jj){
                    Vec1_d avgP (K, 0);
                    ct = 0;
                    for(int iii(max(ii-nn, 0)); iii < min(ii+nn+1,H); ++iii){
                        for(int jjj(max(jj-nn, 0)); jjj < min(jj+n+1,W); ++jjj){
                            for(int k(0); k < K; ++k){
                                avgP[k] += Im3D[0][iii][jjj];
                            }
                            ++ct;
                        }
                    }
                    for(int k(0); k < K; ++k){
                        avgP[k] /= ct;
                    }
                    Vec1_d dRGB (K, 0);
                    Vec1_d tt (K,0);
                    for(int k(0); k < K;++k){
                        dRGB[k] =(avgP[k]-avgPo[k])*(avgP[k]-avgPo[k]);
                    }
                    double dD (exp(-(Sq(i-ii)+Sq(j-jj))/100));
                    for(int k(0); k < K; ++k){
                        tt[k] = exp(-dRGB[k]*dD/sigma2);
                        piV[k] += tt[k]*Im3D[k][ii][jj];
                        sum[k] += tt[k];
                    }
                }
            }
            for(int k(0); k < K; ++k){
                Res[k][i][j] = toUint(piV[k]/sum[k]);
            }
        }
    }
    return Res;
}

Vec3_ui8 MedFilter(C_Vec3_ui8& Im3D, C_i& kernel){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D)), n (kernel>>1);
    Vec3_ui8 Res (K, Vec2_ui8(H, Vec1_ui8(W,0)));;
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            Vec1_i avgPoR, avgPoG, avgPoB;
            for(int ii(max(i-n, 0)); ii < min(i+n+1,H); ++ii){
                for(int jj(max(j-n, 0)); jj < min(j+n+1,W); ++jj){
                    if(K == 1){
                        avgPoR.push_back(Im3D[0][i][j]);}
                    else{
                        avgPoR.push_back(Im3D[0][i][j]);
                        avgPoG.push_back(Im3D[1][i][j]);
                        avgPoB.push_back(Im3D[2][i][j]);
                    }
                }
            }
            int s (Dim1(avgPoR)>>1);
            if(K == 1){
                sort(avgPoR.begin(), avgPoR.end());
                Res[0][i][j] = avgPoR[s];
            }
            else{
                sort(avgPoR.begin(), avgPoR.end());
                sort(avgPoG.begin(), avgPoG.end());
                sort(avgPoB.begin(), avgPoB.end());
                Res[0][i][j] = avgPoR[s];
                Res[1][i][j] = avgPoG[s];
                Res[2][i][j] = avgPoB[s];
            }
        }
    }
    return Res;
}

//In order of RGB
Vec3_ui8 BiDemosaicing(C_Vec3_ui8& Im3D){
    int H (Dim2(Im3D)-1), W (Dim3(Im3D)-1);
    Vec3_ui8 PIm init3_ui8(3, H+1, W+1, 0);
    //4 corners
    PIm[0][0][0] = Im3D[0][0][1];
    PIm[1][0][0] = Im3D[0][0][0];
    PIm[2][0][0] = Im3D[0][1][0];
    
    PIm[0][H][0] = Im3D[0][H-1][1];
    PIm[1][H][0] = (Im3D[0][H-1][0]+Im3D[0][H-1][1])>>1;
    PIm[2][H][0] = Im3D[0][H-1][0];
    
    PIm[0][0][W] = Im3D[0][0][W];
    PIm[1][0][W] = (Im3D[0][0][W-1]+Im3D[0][1][W])>>1;
    PIm[2][0][W] = Im3D[0][1][W-1];
    
    PIm[0][H][W] = Im3D[0][H-1][W];
    PIm[1][H][W] = Im3D[0][H][W];
    PIm[2][H][W] = Im3D[0][H][W-1];
    
    for(int i(1); i < H; ++i){
        if(i%2 == 0){//G in 0th column
            PIm[0][i][0] = Im3D[0][i][1];
            PIm[1][i][0] = Im3D[0][i][0];
            PIm[2][i][0] = (Im3D[0][i-1][0]+Im3D[0][i+1][0])>>1;
            //R in last column
            PIm[0][i][W] = Im3D[0][i][W];
            PIm[1][i][W] = (Im3D[0][i-1][W]+Im3D[0][i+1][W]+Im3D[0][i][W-1])/3;
            PIm[2][i][W] = (Im3D[0][i-1][W-1]+Im3D[0][i+1][W-1])>>1;
        }
        else{//B in 0th column
            PIm[0][i][0] = (Im3D[0][i-1][1]+Im3D[0][i+1][1])>>1;
            PIm[1][i][0] = (Im3D[0][i-1][0]+Im3D[0][i+1][0]+Im3D[0][i][1])/3;
            PIm[2][i][0] = Im3D[0][i][0];
            //G in last column
            PIm[0][i][W] = (Im3D[0][i-1][W]+Im3D[0][i+1][W])>>1;
            PIm[1][i][W] = Im3D[0][i][W];
            PIm[2][i][W] = Im3D[0][i][W-1];
        }
    }
    for(int j(1); j < W; ++j){
        if(j%2 == 0){//G in 0th row
            PIm[0][0][j] = (Im3D[0][0][j-1]+Im3D[0][0][j+1])>>1;
            PIm[1][0][j] = Im3D[0][0][j];
            PIm[2][0][j] = Im3D[0][1][j];
            //B in 255th row
            PIm[0][H][j] = (Im3D[0][H-1][j-1]+Im3D[0][H-1][j+1])>>1;
            PIm[1][H][j] = (Im3D[0][H-1][j]+Im3D[0][H][j-1]+Im3D[0][H][j+1])/3;
            PIm[2][H][j] = Im3D[0][H][j];
        }
        else{//R in 0th row
            PIm[0][0][j] = Im3D[0][0][j];
            PIm[1][0][j] = (Im3D[0][1][j]+Im3D[0][0][j-1]+Im3D[0][0][j+1])/3;
            PIm[2][0][j] = (Im3D[0][1][j-1]+Im3D[0][1][j+1])>>1;
            //G in 255th row
            PIm[0][H][j] = Im3D[0][H-1][j];
            PIm[1][H][j] = Im3D[0][H][j];
            PIm[2][H][j] = (Im3D[0][H][j-1]+Im3D[0][H][j+1])>>1;
        }
    }
    //other pixels
    for(int i(1); i < H; ++i){
        for(int j(1); j < W; ++j){
            if((i+j)%2 == 0){//G
                if(i%2 == 0){//R
                    PIm[0][i][j] = (Im3D[0][i][j-1]+Im3D[0][i][j+1])>>1;
                    PIm[1][i][j] = Im3D[0][i][j];
                    PIm[2][i][j] = (Im3D[0][i-1][j]+Im3D[0][i+1][j])>>1;
                }
                else{//B
                    PIm[0][i][j] = (Im3D[0][i-1][j]+Im3D[0][i+1][j])>>1;
                    PIm[1][i][j] = Im3D[0][i][j];
                    PIm[2][i][j] = (Im3D[0][i][j-1]+Im3D[0][i][j+1])>>1;
                }
            }
            else if(i%2 == 0){//R
                PIm[0][i][j] = Im3D[0][i][j];
                PIm[1][i][j] = (Im3D[0][i-1][j]+Im3D[0][i+1][j]+Im3D[0][i][j-1]+Im3D[0][i][j+1])>>2;
                PIm[2][i][j] = (Im3D[0][i-1][j-1]+Im3D[0][i-1][j+1]+Im3D[0][i+1][j-1]+Im3D[0][i+1][j+1])>>2;
            }
            else{//B
                PIm[0][i][j] = (Im3D[0][i-1][j-1]+Im3D[0][i-1][j+1]+Im3D[0][i+1][j-1]+Im3D[0][i+1][j+1])>>2;
                PIm[1][i][j] = (Im3D[0][i-1][j]+Im3D[0][i+1][j]+Im3D[0][i][j-1]+Im3D[0][i][j+1])>>2;
                PIm[2][i][j] = Im3D[0][i][j];
            }
        }
    }
    return PIm;
}

Vec3_ui8 MHCDemosaicingC(C_Vec3_ui8& Im3D){
    int H (Dim2(Im3D)), W (Dim3(Im3D));
    //kernels
    Vec2_d kernel1 ({{0,0,-0.125,0,0},{0,0,0.25,0,0},{-0.125,0.25,0.5,0.25,-0.125},{0,0,0.25,0,0},{0,0,-0.125,0,0}});
    Vec2_d kernel2 ({{0,0,0.0625,0,0},{0,-0.125,0,-0.125,0},{-0.125,0.5,0.625,0.5,-0.125},{0,-0.125,0,-0.125,0},{0,0,0.0625,0,0}});
    Vec2_d kernel3 ({{0,0,-0.125,0,0},{0,-0.125,0.5,-0.125,0},{0.0625,0,0.625,0,0.0625},{0,-0.125,0.5,-0.125,0},{0,0,-0.125,0,0}});
    Vec2_d kernel4 ({{0,0,-0.1875,0,0},{0,0.25,0,0.25,0},{-0.1875,0,0.75,0,-0.1875},{0,0.25,0,0.25,0},{0,0,-0.1875,0,0}});
    Vec3_ui8 PIm init3_ui8(3, H, W,0);
    //separate 3 color
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
    Vec3_ui8 Res init3_ui8(3, H, W,0);;
    //RG GB RB
    Res[0] = toUint((Add2D(PIm[0], PIm[1], 1)));
    Res[1] = toUint((Add2D(PIm[1], PIm[2], 1)));
    Res[2] = toUint((Add2D(PIm[0], PIm[2], 1)));
    
    Vec3_ui8 tmp init3_ui8(7, H, W, 0);
    tmp[0] = toUint(Conv2D(Res[0], kernel2, true)); //R in G in R row
    tmp[1] = toUint(Conv2D(Res[1], kernel2, true)); //B in G in R row
    
    tmp[2] = toUint(Conv2D(Res[0], kernel3, true)); //R in G in B row
    tmp[3] = toUint(Conv2D(Res[1], kernel3, true)); //B in G in B row
    
    tmp[4] = toUint(Conv2D(Res[2], kernel4, true)); //R in B & B in R
    
    tmp[5] = toUint(Conv2D(Res[0], kernel1, true)); //G in R
    tmp[6] = toUint(Conv2D(Res[1], kernel1, true)); //G in B
    
    Vec3_ui8 R init3_ui8(3, H, W, 0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            if((i+j)%2 == 0){//G
                if(i%2 == 0){//R
                    PIm[0][i][j] = tmp[0][i][j];
                    PIm[2][i][j] = tmp[1][i][j];
                }
                else{
                    PIm[0][i][j] = tmp[2][i][j];
                    PIm[2][i][j] = tmp[3][i][j];
                }
            }
            else if(i%2 == 0){//R
                PIm[1][i][j] = tmp[5][i][j];
                PIm[2][i][j] = tmp[4][i][j];
            }
            else{//B
                PIm[0][i][j] = tmp[4][i][j];
                PIm[1][i][j] = tmp[6][i][j];
            }
        }
    }
    return PIm;
}

Vec3_ui8 Unrolling3D(C_Vec3_ui8& Im3D){
    int H (Dim2(Im3D)), W (Dim3(Im3D));
    Vec3_ui8 Res;
    Vec2_ui8 g;
    for(int i = 0; i < H; ++i){
        Vec1_ui8 l1, l2;
        for(int j(0); j < W; ++j){
            l1.push_back(Im3D[1][i][j]);
            l1.push_back(Im3D[0][i][j]);
            l2.push_back(Im3D[2][i][j]);
            double t(0.299*Im3D[0][i][j]+0.587*Im3D[1][i][j]+0.114*Im3D[2][i][j]);
            l2.push_back(t);
        }
        g.push_back(l1);
        g.push_back(l2);
    }
    Res.push_back(g);
    return Res;
}

Vec3_ui8 BilateralFilter1(C_Vec3_ui8& Im3D, C_i& kernel_size, C_d& sigma){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D)), n (kernel_size>>1);
    double sigma2 (Sq(sigma));
    Vec3_ui8 Res (Im3D);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double sum (0);
            Vec1_d piV (K, 0), avgPo (K, 0);
            for(int ii(i-n); ii < i+n+1; ++ii){
                for(int jj(j-n); jj < j+n+1; ++jj){
                    int iii (ii), jjj (jj);
                    RefectBound(iii, jjj, H, W);
                    double dC (0);
                    if(K == 3){
                        dC = calColorDist(Im3D[0][iii][jjj], Im3D[1][iii][jjj], Im3D[2][iii][jjj], Im3D[0][i][j], Im3D[1][i][j], Im3D[2][i][j])/7;
                    }
                    else{
                        dC = Sq(Im3D[0][iii][jjj]-Im3D[0][i][j]);
                    }
                    double dD ((Sq(i-ii)+Sq(j-jj))/100);
                    double t (exp(-dC/sigma2-dD));
                    sum += t;
                    for(int k = 0; k < K; ++k){
                        piV[k] += t*Im3D[k][iii][jjj];
                    }
                }
            }
            for(int k(0); k < K; ++k){
                Res[k][i][j] = toUint(piV[k]/sum);
            }
        }
    }
    return Res;
}
