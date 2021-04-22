//
//  HW4Basic.cpp
//  EE569
//
//  Created by Alex on 2019/3/4.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HW4Basic.hpp"

map<string, Vec2_d> LawsFilter2D(){
    map<string, Vec2_d> laws1D;
    laws1D["L5"] = {{1, 4, 6, 4, 1}};
    laws1D["E5"] = {{-1, -2, 0, 2, 1}};
    laws1D["S5"] = {{-1, 0, 2, 0, -1}};
    laws1D["W5"] = {{-1, 2, 0, -2, 1}};
    laws1D["R5"] = {{1, -4, 6, -4, 1}};
    map<string, Vec2_d> laws2D;
    map<string, Vec2_d>::iterator it1 = laws1D.begin();
    while(it1 != laws1D.end()){
        map<string, Vec2_d>::iterator it2 = laws1D.begin();
        while(it2 != laws1D.end()){
            string name = it1->first + it2->first;
            laws2D[name] = DotProduct2D(Transpose2D(it1->second), it2->second);
            ++it2;
        }
        ++it1;
    }
    return laws2D;
}

map<string, Vec2_d> TextureExtraction(C_Vec2_d& Im2D){
    map<string, Vec2_d> res;
    map<string, Vec2_d> laws2D = LawsFilter2D();
    map<string, Vec2_d>::iterator it = laws2D.begin();
    while(it != laws2D.end()){
        res[it->first] = Conv2D_Ref(Im2D, it->second, false);
        //Raw2Mat(Normalize2D_Vd2Vd(res[it->first]), str2pchar("texture_"+it->first+"_"+to_string(clock())+".jpg"));
        ++it;
    }
    return res;
}

Vec3_d GetNeededEnergyMap(map<string, Vec2_d>& texture){
    Vec3_d res;
    /*
    res.push_back(Add2D(texture["L5E5"], texture["E5L5"], 1));
    res.push_back(Add2D(texture["L5R5"], texture["R5L5"], 1));
    res.push_back(Add2D(texture["E5S5"], texture["S5E5"], 1));
    res.push_back(texture["S5S5"]);
    res.push_back(texture["R5R5"]);
    res.push_back(Add2D(texture["L5S5"], texture["S5L5"], 1));
    res.push_back(texture["E5E5"]);
    res.push_back(Add2D(texture["E5R5"], texture["R5E5"], 1));
    res.push_back(Add2D(texture["S5R5"], texture["R5S5"], 1));
    */
    map<string, Vec2_d>::iterator it = texture.begin();
    while(it != texture.end()){
        res.push_back(texture[it->first]);
        ++it;
    }
     
    return res;
}

Vec1_d AverageTexture(C_Vec3_d& Im3D){
    int K (Dim1(Im3D));
    double S ((Dim2(Im3D)-2)*(Dim3(Im3D)-2));
    Vec1_d res (K, 0);
    for(int k(0); k < K; ++k){
        res[k] = SqSum2D(Im3D[k], 2)/S;
    }
    return res;
}

Vec2_d myPCA(C_Vec2_d& data, C_i& keepdim){
    Vec2_d res init2_d(Dim1(data), keepdim, 0);
    Mat tmp = Mat(Dim1(data), Dim2(data), CV_32FC1);
    for(int i(0); i < Dim1(data); ++i){
        for(int j(0); j < Dim2(data); ++j){
            tmp.at<float>(i, j) = data[i][j];
        }
    }
    PCA pca(tmp, Mat(), CV_PCA_DATA_AS_ROW, keepdim);
    Mat r = pca.project(tmp);
    for(int i(0); i < Dim1(data); ++i){
        for(int j(0); j < keepdim; ++j){
            res[i][j] = r.at<float>(i, j);
        }
    }
    return res;
}

void NormalizeTexture(Vec2_d& texture){
    int N (Dim1(texture)), D (Dim2(texture));
    //zero mean
    for(int d(0); d < D; ++d){
        double sum (0), ct (0);
        for(int n(0); n < N; ++n){
            sum += texture[n][d];
            ct += 1;
        }
        sum /= ct;
        for(int n(0); n < N; ++n){
            texture[n][d] -= sum;
        }
    }
    //1 std
    for(int d(0); d < D; ++d){
        double sum (0), ct (0);
        for(int n(0); n < N; ++n){
            sum += Sq(texture[n][d]);
            ct += 1;
        }
        sum /= ct;
        sum = sqrt(sum+0.0000001);
        for(int n(0); n < N; ++n){
            texture[n][d] /= sum;
        }
    }
}

void NormalizeTextureLL(Vec3_d& texture, C_Vec2_d& LL){
    int K (Dim1(texture)), H (Dim2(texture)), W (Dim3(texture));
    //zero mean
    for(int k(0); k < K; ++k){
        for(int i(0); i < H; ++i){
            for(int j(0); j < W; ++j){
                texture[k][i][j] /= LL[i][j];
            }
        }
    }
}
