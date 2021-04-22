//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  ImageDenoising.cpp
//  EE569
//
//  Created by Alex on 2019/1/9.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "ImageDenoising.hpp"

Vec3_ui8 getNoise(C_Vec3_ui8& Im3D, C_Vec3_ui8& NoiseIm3D){
    Vec3_ui8 res;
    for(int k(0); k < Dim1(Im3D); ++k){
        res.push_back(toUint(Add2D(Im3D[k], NoiseIm3D[k], 0)));
    }
    return res;
}

Vec3_ui8 UniformFilter(C_Vec3_ui8& Im3D, C_i& kernel_size){
    Vec3_ui8 Res (Im3D);
    Vec2_d UniformCore init2_d(kernel_size, kernel_size, 1);
    for(int k(0); k < Dim1(Im3D); ++k){
        Res[k] = toUint(Conv2D_Ref(Im3D[k], UniformCore, true));
        //Res[k] = Conv2D(Im3D[k], UniformCore, true);
    }
    return Res;
}

Vec3_ui8 GaussianFilter(C_Vec3_ui8& Im3D, C_i& kernel_size, C_d& sigma){
    Vec3_ui8 Res (Im3D);
    Vec2_d CloseCore (genGaussianCore(kernel_size, sigma));
    for(int k(0); k < Dim1(Im3D); ++k){
        Res[k] = toUint(Conv2D_Ref(Im3D[k], CloseCore, true));
        //Res[k] = Conv2D(Im3D[k], CloseCore, true);
    }
    return Res;
}

Vec3_ui8 BilateralFilter(C_Vec3_ui8& Im3D, C_i& kernel_size, C_d& sigmaD, C_d& sigmaR){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D)), n (kernel_size>>1);
    double sigma2 (2 * Sq(sigmaR));
    Vec3_ui8 Res (Im3D);
    Vec2_d CloseCore (genGaussianCore(kernel_size, sigmaD));
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            Vec1_d piV (K, 0);
            double sum (0);
            for(int ii(max(i-n, 0)); ii < min(i+n+1,H); ++ii){
                for(int jj(max(j-n, 0)); jj < min(j+n+1,W); ++jj){
                    double dC = 0;
                    if(K == 3){
                        dC = calColorDist(Im3D[0][i][j], Im3D[1][i][j], Im3D[2][i][j], Im3D[0][ii][jj], Im3D[1][ii][jj], Im3D[2][ii][jj]);
                    }
                    else{
                        dC = Sq(Im3D[0][i][j] - Im3D[0][ii][jj]);
                    }
                    dC = CloseCore[ii-i+n][jj-j+n]*exp(-dC/sigma2);
                    sum += dC;
                    for(int k = 0; k < K; ++k){
                        piV[k] += Im3D[k][ii][jj]*dC;
                    }
                }
            }
            for(int k = 0; k < K; ++k){
                Res[k][i][j] = toUint(piV[k]/sum);
            }
        }
    }
    return Res;
}

Vec3_ui8 NLMeansFilter(C_Vec3_ui8& Im3D, C_d& sigma, C_i& search_window, C_i& compare_window){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D)), n (search_window>>1);
    double sigma2 (Sq(sigma));
    Vec3_d average init3_d(K, H, W, 0);
    Vec2_d unicore init2_d(compare_window, compare_window, 1);
    for(int k(0); k < K; ++k){
        average[k] = Conv2D_Ref(Im3D[k], unicore, true);
    }
    Vec3_ui8 Res (Im3D);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double sum (0);
            Vec1_d piV (K, 0);
            Vec1_d avgPo (K, 0);
            for(int ii(i-n); ii < i+n+1; ++ii){
                for(int jj(j-n); jj < j+n+1; ++jj){
                    int iii(ii), jjj(jj);
                    RefectBound(iii, jjj, H, W);
                    double dC = 0;
                    if(K == 3){
                        dC = calColorDist(average[0][iii][jjj], average[1][iii][jjj], average[2][iii][jjj], average[0][i][j], average[1][i][j], average[2][i][j])/7;
                    }
                    else{
                        dC = Sq(average[0][iii][jjj]-average[0][i][j]);
                    }
                    double dD(exp(-(Sq(i-ii)+Sq(j-jj))/100));
                    double t(exp(-dC*dD/sigma2));
                    sum += t;
                    for(int k(0); k < K; ++k){
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

Vec3_d NLMeansFilter(C_Vec3_d& Im3D, C_d& sigma, C_i& search_window, C_i& compare_window){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D)), n (search_window>>1);
    double sigma2 (Sq(sigma));
    Vec3_d average init3_d(K, H, W, 0);
    Vec2_d unicore init2_d(compare_window, compare_window, 1);
    for(int k(0); k < K; ++k){
        average[k] = Conv2D_Ref(Im3D[k], unicore, true);
    }
    Vec3_d Res (Im3D);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double sum (0);
            Vec1_d piV (K, 0);
            Vec1_d avgPo (K, 0);
            for(int ii(i-n); ii < i+n+1; ++ii){
                for(int jj(j-n); jj < j+n+1; ++jj){
                    int iii (ii), jjj (jj);
                    RefectBound(iii, jjj, H, W);
                    double dC = 0;
                    if(K == 3){
                        dC = calColorDist(average[0][iii][jjj], average[1][iii][jjj], average[2][iii][jjj], average[0][i][j], average[1][i][j], average[2][i][j])/7;
                    }
                    else{
                        dC = Sq(average[0][iii][jjj]-average[0][i][j]);
                    }
                    double dD (exp(-(Sq(i-ii)+Sq(j-jj))/100));
                    double t (exp(-dC*dD/sigma2));
                    sum += t;
                    for(int k(0); k < K; ++k){
                        piV[k] += t*Im3D[k][iii][jjj];
                    }
                }
            }
            for(int k(0); k < K; ++k){
                Res[k][i][j] = piV[k]/sum;
            }
        }
    }
    return Res;
}

Vec3_ui8 ShotNoise(C_Vec3_ui8& Im3D, C_i& kernel_size, C_d& sigma){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D));
    Vec2_d core (genGaussianCore(kernel_size, sigma));
    Vec3_d Res init3_d(K, H, W, 0);
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                Res[k][i][j] = AnscombeRT(Im3D[k][i][j]);
            }
        }
    }
    Vec3_d tmp;
    for(int k(0); k < K; ++k){
        tmp.push_back(Conv2D_Ref(Res[k], core, true));
    }
    Vec3_ui8 Result init3_ui8(K, H, W, 0);
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                Result[k][i][j] = IAnscombeRT(tmp[k][i][j]);
            }
        }
    }
    return Result;
}

Vec3_ui8 NLMeansFilterG(C_Vec3_ui8& Im3D, C_d& sigma, C_d& sigmaG, C_i& search_window, C_i& compare_window){
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D)), n (search_window>>1);
    double sigma2 (Sq(sigma));
    Vec3_d average init3_d(K, H, W, 0);
    Vec2_d guacore (genGaussianCore(compare_window, sigmaG));
    for(int k(0); k < K; ++k){
        average[k] = Conv2D_Ref(Im3D[k], guacore, true);
    }
    Vec3_ui8 Res (Im3D);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double sum (0);
            Vec1_d piV (K, 0);
            Vec1_d avgPo (K, 0);
            for(int ii(i-n); ii < i+n+1; ++ii){
                for(int jj(j-n); jj < j+n+1; ++jj){
                    int iii (ii), jjj (jj);
                    RefectBound(iii, jjj, H, W);
                    double dC(0);
                    if(K == 3){
                        dC = calColorDist(average[0][iii][jjj], average[1][iii][jjj], average[2][iii][jjj], average[0][i][j], average[1][i][j], average[2][i][j])/7;
                    }
                    else{
                        dC = Sq(average[0][iii][jjj]-average[0][i][j]);
                    }
                    double dD (exp(-(Sq(i-ii)+Sq(j-jj))/100));
                    double t (exp(-dC*dD/sigma2));
                    sum += t;
                    for(int k(0); k < K; ++k){
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
