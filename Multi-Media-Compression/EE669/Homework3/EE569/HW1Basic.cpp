//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW1Basic.cpp
//  EE569
//
//  Created by Alex on 2019/1/25.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HW1Basic.hpp"

//save result from getHistogram
void save2DHistVec(C_Vec2_i& db, C_c* name, int K, int H, int W){
    Vec3_ui8 histgram init3_ui8(K, 300, 300, 255);
    double m (0);
    for(int k(0); k < K; ++k){
        for(int i(1); i < 255; ++i){
            m = Max(m, db[k][i]);
        }
    }
    cout<<"save2DHistVec: Freq = "<<m/(H*W)<<endl;
    m = 256/m;
    for(int k(0); k < K; ++k){
        for(int j(0); j < 256; ++j){
            int t(Min(db[k][j]*m,277));
            for(int i(278); i > 278-t; --i){
                histgram[k][i][j+22] = 100;
            }
            histgram[k][279][j+22] = 0;
        }
        for(int i(279); i > 22; --i){
            histgram[k][i][21] = 0;
        }
    }
    WriteRaw(name, histgram);
}

void save2DCumHistVec(C_Vec2_i& db, C_c* name, int K, int H, int W){
    Vec3_ui8 histgram init3_ui8(K, 300, 300, 255);
    double m (0);
    for(int k(0); k < K; ++k){
        for(int i(1); i < 255; ++i){
            m = Max(m, db[k][i]);
        }
    }
    cout<<"save2DCumHistVec: Freq = "<<m/(H*W)<<endl;
    m = 256/m;
    double th (m*H*W/256);
    for(int k(0); k < K; ++k){
        for(int j(0); j < 256; ++j){
            int t(Min(db[k][j]*m,277));
            for(int i(278); i > 278-t; --i){
                histgram[k][i][j+22] = 100;
            }
            histgram[k][279][j+22] = 0;
        }
        for(int i(279); i > 22; --i){
            histgram[k][i][21] = 0;
        }
    }
    for(int k(0); k < K; ++k){
        for(int j(0); j < 256; ++j){
            for(int i(278); i > 278-th; --i){
                histgram[k][i][j+22] = histgram[k][i][j+22]==100? 150:200;
            }
            histgram[k][279][j+22] = 0;
        }
        for(int i(279); i > 22; --i){histgram[k][i][21] = 0;}
    }
    WriteRaw(name, histgram);
}

void saveHistogram(const Vec3_ui8& Im3D, C_c* out_Path){
    Vec2_i db (getHistogram(Im3D));
    save2DHistVec(db, out_Path, Dim1(Im3D), Dim2(Im3D), Dim3(Im3D));
}

Vec2_ui8 HistEqMethodB2D(C_Vec2_ui8& Im2D){
    int H (Dim1(Im2D)), W(Dim2(Im2D));
    int S (H * W), invl (S/256);
    Vec2_ui8 res (Im2D);
    map<int, queue<Vec1_i>> mp;
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            mp[(int)Im2D[i][j]].push({i, j});
        }
    }
    map<int, queue<Vec1_i>>::iterator iter(mp.begin());
    int ct (0), count (0), Cgray (0);
    while(iter != mp.end() && count < S){
        if(iter->second.empty() == true){++iter;}
        if(ct >= invl){
            ct = 0;
            ++Cgray;
        }
        res[iter->second.front()[0]][iter->second.front()[1]] = (uint8_t)Cgray;
        iter->second.pop();
        ++ct;
        ++count;
    }
    return res;
}

Vec2_d genGaussianCore(C_i& kernel_size, C_d& sigma){
    int n(kernel_size>>1);
    double sigma2 (2 * Sq(sigma));
    double sigma2pi (Pi * sigma2);
    Vec2_d kernel init2_d(kernel_size, kernel_size, 0);
    for (int i(-n), row = 0; i <= n; ++i){
        int col(0);
        for (int j(-n); j <= n; ++j){
            kernel[row][col] = exp(-(Sq(i)+Sq(j))/sigma2)/sigma2pi;
            ++col;
        }
        ++row;
    }
    return kernel;
}

float genGaussianCe(C_d& sigma, C_i& i, C_i& j, C_i& ii, C_i& jj, C_b& enlarge){
    double sigma2 (2 * Sq(sigma));
    double xDistance (Sq(i-ii));
    double yDistance (Sq(j-jj));
    if(enlarge == true){
        return exp(-(xDistance+yDistance)/(sigma2*1000));
    }
    return exp(-(xDistance+yDistance)/(sigma2));
}

float genSimilarityCe(C_Vec3_ui8& Im3D, C_d& sigma, C_i& i, C_i& j, C_i& ii, C_i& jj){
    double sigma2 (2*Sq(sigma));
    if(Dim1(Im3D) == 1){
        double EDist (Sq(Im3D[0][i][j] - Im3D[0][ii][jj]));
        return exp(-EDist/sigma2);
    }
    else if(Dim1(Im3D) == 3){
        double dC (calColorDist(Im3D[0][i][j], Im3D[1][i][j], Im3D[2][i][j], Im3D[0][ii][jj], Im3D[1][ii][jj], Im3D[2][ii][jj]));
        return exp(-dC/sigma2);
    }
    else{
        cout<<"genSimilarityCore: channel must be 1 or 3"<<endl;
        exit(1);
    }
}

double AnscombeRT(C_ui8& val){
    return 2*sqrt((double)val+0.375);
}

uint8_t IAnscombeRT(C_d& val){
    return toUint(Sq(val)/4-0.125);
}
