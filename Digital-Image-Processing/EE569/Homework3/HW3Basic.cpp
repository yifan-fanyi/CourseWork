//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW3Basic.cpp
//  EE569
//
//  Created by Alex on 2019/2/4.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HW3Basic.hpp"

Vec2_i DetectCorner(C_Vec2_ui8& Im2D, C_i& threshold){
    int H (Dim1(Im2D)), W (Dim2(Im2D));
    Vec2_ui8 tmp (Im2D);
    Vec2_i res;
    Vec1_i d (4,0);
    for(int i(3); i < H-3; ++i){
        for(int j(3); j < W-3; ++j){
            d[0] = abs(tmp[i][j]-tmp[i-3][j+1]);
            d[1] = abs(tmp[i][j]-tmp[i+3][j-1]);
            d[2] = abs(tmp[i][j]-tmp[i-1][j-3]);
            d[3] = abs(tmp[i][j]-tmp[i+1][j+3]);
            int ct (0);
            for(int p(0); p < 4; ++p){
                if(d[p] > threshold){
                    ct++;
                }
            }
            if(ct >= 3){
                res.push_back({i,j});
            }
        }
    }
    return res;
}

Vec2_i SelectCorner(C_Vec2_i& Corners){
    int N (Dim1(Corners));
    int idx1 (INT_MIN), idx2 (INT_MIN), idx3 (INT_MAX), idx4 (INT_MAX);
    int mi_i (0), mi_j (0), mx_i (0), mx_j (0);
    for(int i(0); i < N; ++i){
        if(idx1 <= Corners[i][0]){
            idx1 = Corners[i][0];
            mx_i = i;
        }
        if(idx2 < Corners[i][1]){
            idx2 = Corners[i][1];
            mx_j = i;
        }
        if(idx3 > Corners[i][0]){
            idx3 = Corners[i][0];
            mi_i = i;
        }
        if(idx4 >= Corners[i][1]){
            idx4 = Corners[i][1];
            mi_j = i;
        }
    }
    return {Corners[mx_i], Corners[mx_j], Corners[mi_i], Corners[mi_j]};
}

Vec2_i DetectCornerHole(C_Vec2_ui8& Im2D){
    int H (Dim1(Im2D)), W (Dim2(Im2D));
    Vec2_i res;
    for(int i(1); i < H-1; ++i){
        for(int j(1); j < W-1; ++j){
            bool a1 = Im2D[i-1][j-1] == 255;
            bool a2 = Im2D[i-1][j] == 255;
            bool a3 = Im2D[i-1][j+1] == 255;
            bool a4 = Im2D[i][j-1] == 255;
            bool a5 = Im2D[i][j] == 255;
            bool a6 = Im2D[i][j+1] == 255;
            bool a7 = Im2D[i+1][j-1] == 255;
            bool a8 = Im2D[i+1][j] == 255;
            bool a9 = Im2D[i+1][j+1] == 255;
            if(!a1&&!a2&&!a3&&!a4&&a5&&a6&&!a7&&a8&&a9){
                res.push_back({i,j});
            }
            if(!a1&&!a2&&!a3&&a4&&a5&&!a6&&a7&&a8&&!a9){
                res.push_back({i,j});
            }
            if(a1&&a2&&!a3&&a4&&a5&&!a6&&!a7&&!a8&&!a7){
                res.push_back({i,j});
            }
            if(!a1&&a2&&a3&&!a4&&a5&&a6&&!a7&&!a8&&!a7){
                res.push_back({i,j});
            }
        }
    }
    return res;
}

int calHoleFillScore(C_Vec2_ui8& Im2D_1, C_Vec2_ui8& Im2D_2, C_i& x1, C_i& y1, C_i& len){
    Vec1_i score (4,0);
    for(int i(0); i < len; i+=4){
        score[0] += Sq(Im2D_1[x1-2][y1+i]-Im2D_2[0][i]);
        score[1] += Sq(Im2D_1[x1-2][y1+i]-Im2D_2[i][0]);
        score[2] += Sq(Im2D_1[x1-2][y1+i]-Im2D_2[len-1][i]);
        score[3] += Sq(Im2D_1[x1-2][y1+i]-Im2D_2[i][len-1]);
    }
    int t (INT_MAX), idx (0);
    for(int i(0); i < 4; ++i){
        if(t > score[i]){
            t = score[i];
            idx = i;
        }
    }
    return idx;
}

void NewtonMethod(double& xDistortion, double& yDistortion, double& xCorrected, double& yCorrected){
    double x0 (xDistortion), y0 (yDistortion);
    for (int j(0); j < 20; j++){
        double r2(Sq(xDistortion)+Sq(yDistortion));
        double distRadialA(1/(1-0.3536*r2+0.1730*Sq(r2)));
        xCorrected = x0*distRadialA;
        yCorrected = y0*distRadialA;
        xDistortion = xCorrected;
        yDistortion = yCorrected;
    }
}
