//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW2Basic.cpp
//  EE569
//
//  Created by Alex on 2019/1/25.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HW2Basic.hpp"

Vec2_i genIndexMatrix(C_i& index, C_b& normalize, C_i& noise){
    Vec2_i I2 ({{1, 2}, {3, 0}});
    Vec2_i res (I2);
    int inde (log2(index));
    for(int ii(0); ii < inde-1; ++ii){
        res = TensorProduct2D<int>({{4}}, res);
        res = TensorAddition2D(I2, res);
    }
    Int_RV;
    RV_Uni UniRD (-abs(noise), abs(noise));
    int H (Dim1(res)), W (Dim2(res));
    double S (255/((double)H*(double)W));
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            if(normalize == true){
                double t(0);
                if(noise != 0){
                    t += UniRD(IntRand);
                }
                t += (double)res[i][j]+0.5;
                t = t*S;
                res[i][j] = t;
            }
        }
    }
    return res;
}

Vec2_ui8 RPSweep2DRefP(C_Vec2_ui8& Im2D, Vec2_d& kernel, C_i& threshold, C_b& normalize){
    int H (Dim1(Im2D)), W (Dim2(Im2D)), n (Dim1(kernel)>>1);
    if(normalize == true){
        Normalize2D(kernel);
    }
    Vec2_ui8 res init2_ui8(H, W, 0);
    Vec2_d err init2_d(H, W, 0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            res[i][j] = (err[i][j]+Im2D[i][j]) > 127 ? 255 : 0;
            err[i][j] += Im2D[i][j] - res[i][j];
            if(threshold > 0){
                err[i][j] = min(threshold, max(-threshold, (int)err[i][j]));
            }
            for(int ii(i); ii <= i+n; ++ii){
                for(int jj(j-n); jj <= j+n; ++jj){
                    int iii (ii), jjj (jj);
                    RefectBound(iii, jjj, H, W);
                    err[iii][jjj] += err[i][j]*kernel[ii-i+n][jj-j+n];
                }
            }
        }
    }
    return res;
}

Vec2_ui8 SPSweep2DRefP(C_Vec2_ui8& Im2D, Vec2_d& kernel, C_i& threshold, C_b& normalize){
    int H (Dim1(Im2D)), W (Dim2(Im2D)), L (Dim1(kernel)), n (L>>1);
    if(normalize == true){
        Normalize2D(kernel);
    }
    Vec2_ui8 res init2_ui8(H, W, 0);
    Vec2_d err init2_d(H, W, 0);
    for(int i(0); i < H; ++i){
        if(i%2 == 0){
            for(int j(0); j < W; ++j){
                res[i][j] = (err[i][j]+Im2D[i][j]) > 127 ? 255 : 0;
                err[i][j] += Im2D[i][j] - res[i][j];
                if(threshold > 0){
                    err[i][j] = min(threshold, max(-threshold, (int)err[i][j]));
                }
                for(int ii(i-n); ii <= i+n; ++ii){
                    for(int jj(j-n); jj <= j+n; ++jj){
                        int iii (ii), jjj (jj);
                        RefectBound(iii, jjj, H, W);
                        err[iii][jjj] += err[i][j]*kernel[ii-i+n][jj-j+n];
                    }
                }
            }
        }
        else{
            for(int j(W-1); j >= 0; --j){
                res[i][j] = (err[i][j]+Im2D[i][j]) > 127 ? 255 : 0;
                err[i][j] += Im2D[i][j] - res[i][j];
                if(threshold > 0){
                    err[i][j] = min(threshold, max(-threshold, (int)err[i][j]));
                }
                for(int ii(i-n); ii <= i+n; ++ii){
                    for(int jj(j-n); jj <= j+n; ++jj){
                        int iii (ii), jjj (jj);
                        RefectBound(iii, jjj, H, W);
                        err[iii][jjj] += err[i][j]*kernel[ii-i+n][L-1-(jj-j+n)];
                    }
                }
            }
        }
    }
    return res;
}

Vec3_ui8 SPSweep3DRefPMBVQ(C_Vec3_ui8& Im3D, Vec2_d& kernel, C_i& threshold, C_b& normalize){
    int H (Dim2(Im3D)), W (Dim3(Im3D)), L (Dim1(kernel)), n (L>>1);
    if(normalize == true){
        Normalize2D(kernel);
    }
    Vec3_ui8 res init3_ui8(3, H, W, 0);
    Vec3_d err init3_d(3, H, W, 0);
    for(int i(0); i < H; ++i){
        if(i%2 == 0){
            for(int j(0); j < W; ++j){
                //Vec1_i Cvertex = ClosetVertex(err[0][i][j]+Im3D[0][i][j], err[1][i][j]+Im3D[1][i][j], err[2][i][j]+Im3D[2][i][j]);
                Vec1_i Cvertex(ClosetVertexDT(err[0][i][j]+Im3D[0][i][j], err[1][i][j]+Im3D[1][i][j], err[2][i][j]+Im3D[2][i][j]));
                for(int k(0); k < 3; ++k){
                    res[k][i][j] = Cvertex[k];
                    err[k][i][j] += Im3D[k][i][j] - res[k][i][j];
                    if(threshold > 0){
                        err[k][i][j] = min(threshold, max(-threshold, (int)err[k][i][j]));
                    }
                    for(int ii(i-n); ii <= i+n; ++ii){
                        for(int jj(j-n); jj <= j+n; ++jj){
                            int iii (ii), jjj (jj);
                            RefectBound(iii, jjj, H, W);
                            err[k][iii][jjj] += err[k][i][j]*kernel[ii-i+n][jj-j+n];
                        }
                    }
                }
            }
        }
        else{
            for(int j(W-1); j >=  0; --j){
                //Vec1_i Cvertex = ClosetVertex(err[0][i][j]+Im3D[0][i][j], err[1][i][j]+Im3D[1][i][j], err[2][i][j]+Im3D[2][i][j]);
                Vec1_i Cvertex (ClosetVertexDT(err[0][i][j]+Im3D[0][i][j], err[1][i][j]+Im3D[1][i][j], err[2][i][j]+Im3D[2][i][j]));
                for(int k(0); k < 3; ++k){
                    res[k][i][j] = Cvertex[k];
                    err[k][i][j] += Im3D[k][i][j] - res[k][i][j];
                    if(threshold > 0){
                        err[k][i][j] = min(threshold, max(-threshold, (int)err[k][i][j]));
                    }
                    for(int ii(i-n); ii <= i+n; ++ii){
                        for(int jj(j-n); jj <= j+n; ++jj){
                            int iii (ii), jjj (jj);
                            RefectBound(iii, jjj, H, W);
                            err[k][iii][jjj] += err[k][i][j]*kernel[ii-i+n][L-1-(jj-j+n)];
                        }
                    }
                }
            }
        }
    }
    return res;
}

Vec2_ui8 ErrDiffusion2D(C_Vec2_ui8& Im2D, C_i& version, C_i& spread, C_i& threshold){
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
        cout<<"ErrorDiffusion: no such kernel version"<<endl;
        exit(1);
    }
    if(spread == 0){
        return RPSweep2DRefP(Im2D, kernel, threshold, true);
    }
    else if(spread == 1){
        return SPSweep2DRefP(Im2D, kernel, threshold, true);
    }
    else if(spread == 2){
        if(version != 1){
            return HW2Fancy2(Im2D, version);
        }
        else{
            cout<<"ErrDiffusion2D: Sorry do not support JJN kernel."<<endl;
            exit(1);
        }
    }
    else{
        cout<<"ErrDiffusion2D: no such spread method"<<endl;
        exit(1);
    }
}

string MBVQ(C_i& R, C_i& G, C_i& B){
    if(R+G > 255){
        if(G+B > 255){
            if(R+B+G > 510){return "CMYW";}
            else{return "MYGC";}
        }
        else{return "RGMY";}
    }
    else{
        if(!(G+B > 255)){
            if(!(R+B+G > 255)){return "KRGB";}
            else{return "RGBM";}
        }
        else{return "CMGB";}
    }
}

map<char, Vec1_i> BuildCube(){
    map<char, Vec1_i> mp;
    mp['K'] = {0, 0, 0};
    mp['R'] = {255, 0, 0};
    mp['G'] = {0, 255, 0};
    mp['B'] = {0, 0, 255};
    mp['C'] = {0, 255, 255};
    mp['M'] = {255, 0, 255};
    mp['Y'] = {255, 255, 0};
    mp['W'] = {255, 255, 255};
    return mp;
}

Vec1_i ClosetVertex(C_i& R, C_i& G, C_i& B){
    map<char, Vec1_i> mp(BuildCube());
    string s (MBVQ(R, G, B));
    int mindist (INT_MAX);
    int index (-1);
    for(int i(0); i < 4; ++i){
        int t (abs(R-mp[s[i]][0])+abs(G-mp[s[i]][1])+abs(B-mp[s[i]][2]));
        if(t < mindist){
            mindist = t;
            index = i;
        }
    }
    return mp[s[index]];
}

Vec1_i ClosetVertexDT(C_i& R, C_i& G, C_i& B){
    map<char, Vec1_i> mp(BuildCube());
    string s (MBVQ(R, G, B));
    char vertex;
    if(s == "CMYW"){
        vertex = 'W';
        if(B < 127){
            if(B <= R){
                if(B <= G){
                    vertex = 'Y';}}}
        if(G < 127){
            if (G <= B){
                if (G <= R){
                    vertex = 'M';}}}
        if(R < 127){
            if (R <= B){
                if (R <= G){
                    vertex = 'C';}}}
    }
    else if(s == "MYGC"){
        vertex = 'M';
        if(G >= B){
            if(R >= B){
                if(R >= 127){
                    vertex = 'Y';}
                else{
                    vertex = 'G';}}}
        if(G >= R){
            if(B >= R){
                if(B >= 127){
                    vertex = 'C';}
                else{
                    vertex = 'G';}}}
    }
    else if(s == "RGMY"){
        if(B > 127){
            if(R > 127){
                if(B >= G){
                    vertex = 'M';}
                else{
                    vertex = 'Y';}}
            else{
                if(G > B + R){
                    vertex = 'G';}
                else{
                    vertex = 'M';}}}
        else{
            if(R >= 127){
                if(G >= 127){
                    vertex = 'Y';}
                else{
                    vertex = 'R';}}
            else{
                if(R >= G){
                    vertex = 'R';}
                else{
                    vertex = 'G';}}}
    }
    else if(s == "KRGB"){
        vertex = 'K';
        if(B > 127){
            if(B >= R){
                if(B >= G){
                    vertex = 'B';}}}
        if(G > 127){
            if(G >= B){
                if(G >= R){
                    vertex = 'G';}}}
        if(R > 127){
            if(R >= B){
                if(R >= G){
                    vertex = 'R';}}}
    }
    else if(s == "RGBM"){
        vertex = 'G';
        if(R > G){
            if(R >= B){
                if(B < 127){
                    vertex = 'R';}
                else{
                    vertex = 'M';}}}
        if(B > G){
            if(B >= R){
                if(R < 127){
                    vertex = 'B';}
                else{
                    vertex = 'M';}}}
    }
    else if (s == "CMGB"){
        if(B > 127){
            if( R > 127){
                if(G >= R){
                    vertex = 'C';}
                else{
                    vertex = 'M';}}
            else{
                if (G > 127){
                    vertex = 'C';}
                else{
                    vertex = 'B';}}}
        else{
            if(R > 127){
                if (R - G + B >= 127){
                    vertex = 'M';}
                else{
                    vertex = 'G';}}
            else{
                if(G >= B){
                    vertex = 'G';}
                else{
                    vertex = 'B';}}}
    }
    return mp[vertex];
}
