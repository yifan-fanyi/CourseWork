//
//  HW3_utli.hpp
//  EE669
//
//  Created by Alex on 2019/10/2.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW3_utli_hpp
#define HW3_utli_hpp

#include "utli.hpp"

vector<vector<double>> imread(const string& name){
    ifstream f(name, ios_base::binary);
    vector<unsigned char> v((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    vector<vector<double>> data(v.size(), vector<double>(1, 0));
    for(int i = 0; i < v.size(); ++i){
        data[i][0] = (unsigned int)v[i];
    }
    return data;
}

Vec2_d KmeansInitPDF256(C_Vec2_d& data, C_i& N){
    Vec2_d cent;
    vector<int> bin(256,0);
    for(int i = 0; i < data.size(); ++i){
        bin[data[i][0]] += 1;
    }
    int sum = 0;
    double t = (double)data.size()/(double)(N);
    double last = 0;
    double lastsum = 0;
    for(int i(0); i < 256; ++i){
        sum += bin[i];
        if(sum >= t){
            double tt = last*lastsum;
            for(int j = last+1; j < i; ++j){
                tt += (double)j*(double)bin[j];
            }
            tt += (double)i*(bin[i]-(sum-t));
            cent.push_back({(double)tt/t});
            sum -= t;
            lastsum = sum;
            last = i;
        }
    }
    return cent;
}

Vec2_d Kmeans(C_Vec2_d& data, C_i& N, int iter){
    int dim = 1;
    Vec2_d center;
    center = KmeansInitPDF256(data, N);
    Vec1_i label(Dim1(data), 0);
    bool modify = true;
    while(iter > 0){
        modify = false;
        KmeansCluster(data, center, label, dim, modify);
        if(modify == false){
            break;
        }
        iter--;
    }
    return center;
}

Vec1_d find_cloest_center(C_Vec1_d& val, C_Vec2_d& center){
    Vec1_d res;
    double current_min = INT_MAX;
    for(int i = 0; i < Dim1(center); ++i){
        double tmp = 0;
        for(int j = 0; j < Dim2(center); ++j){
            tmp += (val[j]-center[i][j])*(val[j]-center[i][j]);
        }
        if(tmp < current_min){
            res = center[i];
            current_min = tmp;
        }
    }
    return res;
}

double calPSNR(C_Vec2_d& orgImg, C_Vec2_d& proImg){
    int H(Dim1(orgImg));
    int W(Dim2(orgImg));
    double MSE(0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            MSE += (float)Sq(orgImg[i][j]-proImg[i][j]);
        }
    }
    cout<<"MSE: "<<MSE/(H*W)<<endl;
    MSE /= (Sq(255));
    double m(H*W);
    double PSNR(10*log10(m/MSE));
    cout<<"PSNR: "<<PSNR<<endl;
    return MSE;
}

#endif /* HW3_utli_hpp */
