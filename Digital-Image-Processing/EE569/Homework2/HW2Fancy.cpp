//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW2Fancy.cpp
//  EE569
//
//  Created by Alex on 2019/1/28.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HW2Fancy.hpp"

Vec3_ui8 HW2Fancy1(C_Vec3_ui8& Im3D, C_i& window, C_i& method){
    Vec3_ui8 t0;
    if(method == 0){
        t0 = DitheringMatrix(Im3D, 32, 0);
    }
    else if(method == 1){
        t0 = ErrorDiffusion(Im3D, 1, 1, true, -1);
    }
    else if(method == 2){
        t0 = MBVQErrDiff(Im3D, 1, 59);
    }
    else{
        cout<<"HW2Fancy1: method error"<<endl;
        exit(1);
    }
    t0 = Mess(t0, window);
    return t0;
}

Vec2_ui8 HW2Fancy2(C_Vec2_ui8& Im2D, C_i& version){
    int H (Dim1(Im2D)), W (Dim2(Im2D));
    int n (max(log2(H), log2(W))+1);
    int k (pow(4,n));
    Vec2_d kernel1, kernel2, kernel3, kernel4;
    if(version == 0){
        kernel1 = {{0, 0, 0}, {0, 0, 0.4375}, {0.1875, 0.3125, 0.0625}};
        kernel2 = {{0, 0, 0}, {0.4375, 0, 0}, {0.1875, 0.3125, 0.0625}};
        kernel3 = {{0, 0.4375, 0.1875}, {0, 0, 0.3125}, {0, 0, 0.0625}};
        kernel4 = {{0, 0, 0.1875}, {0, 0, 0.3125}, {0, 0.4375, 0.0625}};
    }
    else{
        kernel1 = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0.190476, 0.0952381}, {0.047619, 0.0952381, 0.190476, 0.0952381, 0.047619}, {0.0238095, 0.047619, 0.0952381, 0.047619, 0.0238095}};
        kernel2 = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0.0952381, 0.190476, 0, 0, 0}, {0.047619, 0.0952381, 0.190476, 0.0952381, 0.047619}, {0.0238095, 0.047619, 0.0952381, 0.047619, 0.0238095}};
        kernel3 = {{0, 0, 0, 0.047619, 0.0238095}, {0, 0, 0, 0.0952381, 0.047619}, {0, 0, 0, 0.190476, 0.0952381}, {0, 0, 0.190476, 0.0952381, 0.047619}, {0, 0, 0.0952381, 0.047619, 0.0238095}};
        kernel4 = {{0, 0, 0.0952381, 0.047619, 0.0238095}, {0, 0, 0.190476, 0.0952381, 0.047619}, {0, 0, 0, 0.190476, 0.0952381}, {0, 0, 0, 0.0952381, 0.047619}, {0, 0, 0, 0.047619, 0.0238095}};
    }
    int nn (Dim1(kernel1)>>1);
    Vec2_ui8 res (Im2D);
    Vec2_d err init2_d(H, W, 0);
    int i (0), j (0), ni (0), nj (0);
    for(int index(0); index < k; ++index){
        hilbertIndexToXY(n, index, i, j);
        if(i >= H || j >= W){continue;}
        hilbertIndexToXY(n, index+1, ni, nj);
        int xdir (ni - i);
        int ydir (nj - j);
        res[i][j] = (err[i][j]+Im2D[i][j]) > 127 ? 255 : 0;
        err[i][j] += Im2D[i][j] - res[i][j];
        for(int ii(i-nn); ii < i+nn+1; ++ii){
            for(int jj(j-nn); jj < j+nn+1; ++jj){
                int iii (ii), jjj (jj);
                RefectBound(iii, jjj, H, W);
                if(ydir == 1){
                    err[iii][jjj] += err[i][j]*kernel1[ii-i+nn][jj-j+nn];
                }
                else if(ydir == -1){
                    err[iii][jjj] += err[i][j]*kernel2[ii-i+nn][jj-j+nn];
                }
                else if(xdir == 1){
                    err[iii][jjj] += err[i][j]*kernel3[ii-i+nn][jj-j+nn];
                }
                else if(xdir == -1){
                    err[iii][jjj] += err[i][j]*kernel4[ii-i+nn][jj-j+nn];
                }
            }
        }
    }
    return res;
}

Vec1_i genRandomSeq(C_i& length){
    map<int, int> mp;
    Vec1_i res;
    Int_RV;
    RV_Uni UniRD (0, length-1);
    int ct (0);
    while(ct != length){
        int t(UniRD(IntRand));
        if(mp[t] == 0){
            mp[t] = 1;
            res.push_back(t);
            ++ct;
        }
    }
    return res;
}

Vec3_ui8 Mess(C_Vec3_ui8& Im3D, C_i& window){
    Vec3_ui8 res (Im3D);
    int K (Dim1(Im3D)), H (Dim2(Im3D)), W (Dim3(Im3D));
    int length (Sq(window));
    Vec1_i seq;
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; i+=window){
            for(int j(0); j < W; j+=window){
                seq = genRandomSeq(length);
                for(int p(0); p < length; ++p){
                    int ii (i+seq[p]/window);
                    int jj (j+seq[p]%window);
                    int iii (i+p/window);
                    int jjj (j+p%window);
                    RefectBound(iii, jjj, H, W);
                    res[k][ii][jj] = Im3D[k][iii][jjj];
                }
            }
        }
    }
    return res;
}

Vec3_ui8 Stack3D(C_Vec3_ui8& Im3D){
    int H (Dim2(Im3D)), W (Dim3(Im3D));
    Vec3_ui8 res init3_ui8(3, H>>1, W>>1, 0);
    for(int i(0); i < H; i+=2){
        for(int j(0); j < W; j+=2){
            res[1][i>>1][j>>1] = Im3D[0][i][j];
            res[0][i>>1][j>>1] = Im3D[0][i][j+1];
            res[2][i>>1][j>>1] = Im3D[0][i+1][j];
        }
    }
    return res;
}

void runGetIndex(int n, Vec2_i& mat){
    if(n == 2){
        return;
    }
    int len (Dim1(mat));
    for(int i(0); i < len; ++i){
        Vec1_i tmp(len<<1, 0);
        for(int j(0); j < len; ++j){
            tmp[j] = (mat[i][j]<<2)+2;
            tmp[j+len] = mat[i][j]<<2;
            mat[i].push_back((mat[i][j]<<2)+3);
            mat[i][j] = (mat[i][j]<<2)+1;
        }
        mat.push_back(tmp);
    }
    runGetIndex(n>>1, mat);
}

Vec2_i getIndexRec(int n){
    Vec2_i index ({{1,2},{3,0}});
    runGetIndex (n, index);
    return index;
}
