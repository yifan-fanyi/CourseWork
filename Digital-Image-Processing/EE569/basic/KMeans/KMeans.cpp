//
//  KMeans.cpp
//  EE569
//
//  Created by Alex on 2019/3/4.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "KMeans.hpp"

Vec2_d KmeansInitMaxD(C_Vec2_d& data, C_i& N, C_i& dim){
    Vec2_d cent;
    map<int,int> mp;
    cent.push_back(data[0]);
    mp[0] =1;
    for(int i(1); i < N; ++i){
        double mxd(0), index(0);
        for(int j(0); j < Dim1(data); ++j){
            double dist(0);
            for(int k(0); k < Dim1(cent); ++k){
                for(int p(0); p < dim; ++p){
                    dist += Sq(cent[k][p]-data[j][p]);
                }
            }
            if(dist > mxd && mp[j] == 0){
                mxd = dist;
                index = j;
                mp[j] = 1;
            }
        }
        cent.push_back(data[index]);
    }
    return cent;
}

Vec2_d KmeansInitUni(C_Vec2_d& data, C_i& N){
    Vec2_d cent;
    map<int,int> mp;
    Int_RV;
    RV_Uni UniRD(0, Dim1(data)-1);
    for(int i(0); i < N;){
        int t = UniRD(IntRand);
        if(mp[t] == 0){
            cent.push_back(data[t]);
            mp[t] = 1;
            ++i;
        }
    }
    return cent;
}

void KmeansCluster(C_Vec2_d& data, Vec2_d& center, Vec1_i& label, C_i& dim, bool& modified){
    map<int, Vec2_d> tmp;
    Vec2_d newCent init2_d(Dim1(center), dim+1, 0);
    for(int i(0); i < Dim1(data); ++i){
        double mindist = INT_MAX;
        int lab(0);
        for(int j(0); j < Dim1(center); ++j){
            double dist(0);
            for(int ii(0); ii < min(dim, Dim2(data)); ++ii){
                dist += Sq(data[i][ii]-center[j][ii]);
            }
            if(dist < mindist){
                lab = j;
                mindist = dist;
            }
        }
        if(label[i] != lab){
            label[i] = lab;
            modified = true;
        }
        for(int ii(0); ii < dim; ++ii){
            newCent[lab][ii] += data[i][ii];
        }
        newCent[lab][dim] += 1;
    }
    for(int i(0); i < Dim1(center); ++i){
        for(int ii(0); ii < dim; ++ii){
            center[i][ii] = newCent[i][ii]/newCent[i][dim];
        }
    }
}

Vec1_i Kmeans(C_Vec2_d& data, C_i& N, C_i& dim, C_i& init){
    Vec2_d center;
    if(init == 1){
        center = KmeansInitMaxD(data, N, dim);
    }
    else{
        center = KmeansInitUni(data, N);
    }
    Vec1_i label(Dim1(data), 0);
    bool modify = true;
    while(1){
        modify = false;
        KmeansCluster(data, center, label, dim, modify);
        if(modify == false){
            break;
        }
    }
    return label;
}




