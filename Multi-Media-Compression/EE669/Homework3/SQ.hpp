//
//  SQ.hpp
//  EE669
//
//  Created by Alex on 2019/10/2.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef SQ_hpp
#define SQ_hpp

#include "HW3_utli.hpp"

void Histgram(const string& name,  const char* save, const int H, const int W, const int K){
    vector<vector<vector<int>>> Im3D = imread(name, K, H, W);
    WriteRaw("tmp.raw", Im3D);
    double S (H * W);
    S = 255/S;
    Vec2_i db (getHistogram(Im3D));
    for(int i(0); i < 256; ++i){
        double val = db[0][i];
        val = val*255/S;
        db[0][i] = (int)val;
    }
    save2DHistVec(db, save, 1, H, W);
}

vector<vector<double>> SQ_train(vector<vector<double>>& data, const int bit, int iter){
    int N = pow(2,bit);
    return Kmeans(data, N, iter);
}

double SQ_test(Vec2_d& data, Vec2_d& center, const char* save){
    Vec2_d res init2_d(Dim1(data), Dim2(data), 0);
    for(int i = 0; i < Dim1(data); ++i){
        res[i] = find_cloest_center(data[i], center);
    }
    WriteRaw(save, res);
    return calPSNR(data, res);
}

double SQ_run(vector<string>& train, string& name, const char* save, const int bit, int iter){
    vector<vector<double>> x_train = imread(train[0]);
    for(int i = 1; i < train.size(); ++i){
        vector<vector<double>> tmp = imread(train[i]);
        x_train.insert(x_train.end(),tmp.begin(),tmp.end());
    }
    vector<vector<double>> center = SQ_train(x_train, bit, iter);
    vector<vector<double>> tmp = imread(name);
    return SQ_test(tmp, center, save);
}
#endif /* SQ_hpp */
