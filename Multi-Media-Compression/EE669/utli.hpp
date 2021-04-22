//
//  utli.hpp
//  EE669
//
//  Created by Alex on 2019/10/14.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef utli_hpp
#define utli_hpp

#define pi 3.1415926
#include "Homework3/EE569/HW1Basic.hpp"

vector<vector<vector<int>>> imread(const string& name, const int K, const int H, const int W){
    ifstream f(name, ios_base::binary);
    vector<unsigned char> v((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    vector<vector<vector<int>>> data(K, vector<vector<int>>(H, vector<int>(W, 0)));
    for(int i = 0, k = 0, h = 0, w = 0; i < v.size(); ++i){
        data[k][h][w] = (unsigned int)v[i];
        ++k;
        if(k == K){
            k = 0;
            ++w;
        }
        if(w == W){
            w = 0;
            ++h;
        }
        if(h == H){
            break;
        }
    }
    return data;
}

vector<vector<vector<double>>> imread_d(const string& name, const int K, const int H, const int W){
    ifstream f(name, ios_base::binary);
    vector<unsigned char> v((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    vector<vector<vector<double>>> data(K, vector<vector<double>>(H, vector<double>(W, 0)));
    for(int i = 0, k = 0, h = 0, w = 0; i < v.size(); ++i){
        data[k][h][w] = (unsigned int)v[i];
        ++k;
        if(k == K){
            k = 0;
            ++w;
        }
        if(w == W){
            w = 0;
            ++h;
        }
        if(h == H){
            break;
        }
    }
    return data;
}

vector<vector<vector<double>>> block2D(const vector<vector<double>>& data, const int& block){
    vector<vector<vector<double>>> res;
    for(int i = 0; i < (int)data[0].size() / block; ++i){
        for(int j = 0; j < (int)data[1].size() / block; ++j){
            vector<vector<double>> tmpb;
            for(int ii = block*i; ii < block*(i+1); ++ii){
                vector<double> tmp;
                for(int jj = block*j; jj < block*(j+1); ++jj){
                    tmp.push_back(data[ii][jj]);
                }
                tmpb.push_back(tmp);
            }
            res.push_back(tmpb);
        }
    }
    return res;
}
#endif /* utli_hpp */
