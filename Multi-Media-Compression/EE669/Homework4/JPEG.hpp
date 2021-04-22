//
//  JPEG.hpp
//  EE669
//
//  Created by Alex on 2019/10/14.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef JPEG_hpp
#define JPEG_hpp

#include "utli.hpp"

const vector<vector<double>> Q50 = {
    {16,11,10,16,24,40,51,61},
    {12,12,14,19,26,58,60,55},
    {14,13,16,24,40,57,69,56},
    {14,17,22,29,51,87,80,62},
    {18,22,37,56,68,109,103,77},
    {24,35,55,64,81,104,113,92},
    {49,64,78,87,103,121,120,101},
    {72,92,95,98,112,100,103,99}
};

vector<vector<double>> GenerateQ(int N){
    vector<vector<double>> res (8, vector<double>(8, 0));
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(N > 50){
                res[i][j] = (100. - (double)N) / 50. * Q50[i][j];
            }
            else{
                res[i][j] = 50. / (double)N * Q50[i][j];
            }
        }
    }
    return res;
}

vector<vector<double>> RemoveVal(const vector<vector<double>>& data, const double& val){
    vector<vector<double>> res (data.size(), vector<double>(data[0].size(), 0));
    for(int i = 0; i < data.size(); ++i){
        for(int j = 0; j < data[i].size(); ++j){
            res[i][j] = data[i][j] - val;
        }
    }
    return res;
}

vector<vector<double>> EWDivide(const vector<vector<double>>& data1, const vector<vector<double>>& data2){
    vector<vector<double>> res (data1.size(), vector<double>(data1[0].size(), 0));
    for(int i = 0; i < data1.size(); ++i){
        for(int j = 0; j < data1[i].size(); ++j){
            res[i][j] = data1[i][j]/data2[i][j];
        }
    }
    return res;
}

int Round(double x){
    if(x < 0){
        return (int)(x-0.5);
    }
    return (int)(x+0.5);
}

vector<vector<double>> DCT(const vector<vector<double>>& data){
    vector<vector<double>> dat = RemoveVal(data, 128);
    double N = (double)data[1].size();
    vector<vector<double>> res (N, vector<double>(N, 0));
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < N; ++j){
            double ci = sqrt(2/N), cj = sqrt(2/N);
            if(i == 0){
                ci = sqrt(1/N);
            }
            if(j == 0){
                cj = sqrt(1/N);
            }
            double tmp = 0;
            for(int ii = 0; ii < N; ++ii){
                for(int jj = 0; jj < N; ++jj){
                    tmp += dat[ii][jj] * cos((2*ii+1)*i*pi/(2*N)) * cos((2*jj+1)*j*pi/(2*N));
                }
            }
            res[i][j] = ci * cj * tmp;
        }
    }
    return res;
}

vector<vector<int>> QuantizedDCT(const vector<vector<double>>& data, int Q){
    vector<vector<double>> qdct = EWDivide(DCT(data), GenerateQ(Q));
    vector<vector<int>> res (data.size(), vector<int>(data[0].size(), 0));
    for(int i = 0; i < data.size(); ++i){
        for(int j = 0; j < data[0].size(); ++j){
            res[i][j] = Round(qdct[i][j]);
        }
    }
    return res;
}

double Local_Mean(const vector<vector<vector<double>>>& data, int k, int i, int j, int dh, int dw){
    double sum = 0;
    int ct = 0;
    for(int ii = -dh; ii <= dh; ++ii){
        for(int jj = -dw; jj <= dw; ++jj){
            sum += data[k][i+ii][j+jj];
            ++ct;
        }
    }
    return sum/(double)ct;
}

double Local_SmiMean(const vector<vector<vector<double>>>& data, int k, int i, int j, int dh, int dw){
    double sum = 0;
    int ct = 0;
    for(int ii = -dh; ii <= dh; ++ii){
        for(int jj = -dw; jj <= dw; ++jj){
            if(abs(data[k][i][j]-data[k][i+ii][j+jj])<10){
                sum += data[k][i+ii][j+jj];
                ++ct;
            }
        }
    }
    return sum/(double)ct;
}

vector<vector<vector<double>>> Deblocking_A(const vector<vector<vector<double>>>& data){
    vector<vector<vector<double>>> res(data);
    for(int k = 0; k < data.size(); ++k){
        for(int i = 8; i < data[0].size()-8; ++i){
            for(int j = 8; j < data[0][0].size()-8; ++j){
                int t1 = 1,t2 = 2;
                int w = 2;
                
                if(i % 8 == 0 && j % 8 == 0){
                    res[k][i][j] = Local_SmiMean(data, k, i, j, w, w);
                    res[k][i-1][j] = Local_SmiMean(data, k, i-1, j, w, w);
                    res[k][i-1][j-1] = Local_SmiMean(data, k, i-1, j-1, w, w);
                    res[k][i][j-1] = Local_SmiMean(data, k, i, j-1, w, w);
                }
                else if(i % 8 == 0 && j % 8 != 1 && j % 8 != 7){
                    res[k][i][j] = Local_SmiMean(data, k, i, j, t2, t1);
                    res[k][i-1][j] = Local_SmiMean(data, k, i-1, j, t2, t1);
                }
                else if(j % 8 == 0 && i % 8 != 1 && i % 8 != 7){
                    res[k][i][j] = Local_SmiMean(data, k, i, j, t1, t2);
                    res[k][i-1][j] = Local_SmiMean(data, k, i-1, j, t1, t2);
                }
            }
        }
    }
    return res;
}

void Deblocking_A_run(string name, int H, int W, int K){
    vector<vector<vector<double>>> data = imread_d(name + ".raw", K, H, W);
    vector<vector<vector<double>>> res = Deblocking_A(data);
    WriteRaw(str2pchar(name+"_deblock.raw"), res);
    calPSNR(str2pchar(name+".raw"), str2pchar(name+"_deblock.raw"), K, H, W);
    calSSIM(str2pchar(name+".raw"), str2pchar(name+"_deblock.raw"), K, H, W);
}

vector<vector<vector<double>>> ZeroPadding(vector<vector<vector<double>>>& data, vector<int> pad){
    vector<vector<vector<double>>> res(data);
    for(int k = 0; k < data.size(); ++k){
        for(int i = 0; i < data[0].size(); ++i){
            for(int p = 0; p < pad[0]; ++p){
                res[k][i].insert(res[k][i].begin(), 0);
            }
            for(int p = 0; p < pad[1]; ++p){
                res[k][i].push_back(0);
            }
        }
        vector<double> tmp(res[k][0].size(), 0);
        for(int p = 0; p < pad[2]; ++p){
            res[k].insert(res[k].begin(), tmp);
        }
        for(int p = 0; p < pad[3]; ++p){
            res[k].push_back(tmp);
        }
    }
    return res;
}

#endif /* JPEG_hpp */
