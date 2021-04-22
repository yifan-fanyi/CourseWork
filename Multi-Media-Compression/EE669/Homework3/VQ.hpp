//
//  VQ.hpp
//  EE669
//
//  Created by Alex on 2019/10/5.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef VQ_hpp
#define VQ_hpp

#include "HW3_utli.hpp"

vector<vector<double>> imread_block(const string& name, const int H, const int W, const int block){
    vector<vector<vector<int>>> Im3D = imread(name, 1, H, W);
    vector<vector<double>> res;
    int n = H / block;
    int m = W / block;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            vector<double> tmp;
            for(int ii = block*i; ii < block*(i+1); ++ii){
                for(int jj = block*j; jj < block*(j+1); ++jj){
                    tmp.push_back((double)Im3D[0][ii][jj]);
                }
            }
            res.push_back(tmp);
        }
    }
    return res;
}

vector<vector<double>> imwrite_block(vector<vector<double>>& data, const int H, const int W, const int block){
    vector<vector<double>> res(H, vector<double>(W, 255));
    int n = H / block;
    int m = W / block;
    int t = 0;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            int tt = 0;
            for(int ii = block*i; ii < block*(i+1); ++ii){
                for(int jj = block*j; jj < block*(j+1); ++jj){
                    res[ii][jj] = data[t][tt];
                    ++tt;
                }
            }
            ++t;
        }
    }
    return res;
}

void VQ_Block_run(vector<string>& train, const int block){
    vector<vector<double>> x_train = imread_block(train[0], 256, 256, block);
    for(int i = 1; i < train.size(); ++i){
        vector<vector<double>> tmp = imread_block(train[i], 256, 256, block);
        x_train.insert(x_train.end(),tmp.begin(),tmp.end());
    }
    WriteRaw("block.raw", x_train);
    ofstream ofile;
    ofile.open("block.txt");
    for(int i = 0; i < x_train.size(); ++i){
        for(int j = 0; j < x_train[0].size(); ++j){
            ofile<<(unsigned char)x_train[i][j];
        }
    }
    ofile.close();
}

void VQ_Deblock_run(string& name, string& save, const int block){
    vector<vector<vector<double>>> tmp = imread_d(name, 1, 65536/(block*block), block*block);
    vector<vector<double>> res = imwrite_block(tmp[0], 256, 256, block);
    WriteRaw(str2pchar(save), res);
}

#endif /* VQ_hpp */
