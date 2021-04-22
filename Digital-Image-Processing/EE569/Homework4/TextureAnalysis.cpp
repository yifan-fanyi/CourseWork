//
//  TextureAnalysis.cpp
//  EE569
//
//  Created by Alex on 2019/3/4.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "TextureAnalysis.hpp"

Vec2_d GetAllTexture(){
    Vec2_d res;
    for(int i(1); i <= 12; ++i){
        string name = "texture"+to_string(i)+".raw";
        Vec3_ui8 Im3D = ReadRaw(str2pchar(name), 1, 128, 128);
        Im3D = HistEqMethodA(Im3D);
        Vec2_d tmp = toDouble(Im3D[0]);
        double mean = Sum2D(tmp)/(Sq(128));
        tmp = Add2D_1(tmp, mean, -1);
        map<string, Vec2_d> tmpF = TextureExtraction(tmp);
        Vec3_d fea = GetNeededEnergyMap(tmpF);
        Vec1_d t = AverageTexture(fea);
        res.push_back(t);
    }
    NormalizeTexture(res);
    return res;
}

Vec2_i TextureLabel(C_i& init){
    Vec2_i res;
    Vec2_d tmp = GetAllTexture();
    for(int i(0); i < 25; ++i){
        cout<<tmp[0][i]<<endl;
    }
    Vec1_i label = Kmeans(tmp, 4, 9, init);
    res.push_back(label);
    Vec2_d tmp1 = myPCA(tmp, 3);
    for(int i(0); i < Dim1(tmp1); ++i){
        for(int j(0); j < Dim2(tmp1); ++j){
            cout<<tmp1[i][j]<<"  ";
        }
        cout<<endl;
    }
    cout<<endl;
    label = Kmeans(tmp1, 4, 3, init);
    res.push_back(label);
    return res;
}

void DrawSortTextureMap(C_Vec2_d& texture){
    Vec2_d fm;
    Vec1_i seq ({1,5,7,2,8,10,3,9,11,4,6,12});
    for(int i(0); i < Dim1(seq); ++i){
        fm.push_back(texture[seq[i]-1]);
    }
    Raw2Mat(fm, "sorted_texturemap.jpg");
}

Vec2_i TextureSeparate(C_Vec3_ui8& Im3D, C_i& window, C_i& init){
    Vec2_i res;
    Vec2_d tmp = GetTextureWin(Im3D, window);
    //Vec2_d tmp = GetTextureWin1(Im3D, window);
    Vec1_i label;
    Vec2_d tmp1 = myPCA(tmp, 3);
    label = Kmeans(tmp1, 7, 3, init);
    res.push_back(label);
    DrawSegImg(Im3D, label, window);
    //DrawSegImg1(Im3D, label, window);
    return res;
}

Vec2_i TextureSeparate1(C_Vec3_ui8& Im3D, C_i& window, C_i& init){
    Vec2_i res;
    int n = NumIterTS;
    int H (Dim2(Im3D)), W (Dim3(Im3D));
    Vec2_d tmp = GetTextureWin1(Im3D, window);
    //Vec2_d tmp = GetTextureWin(Im3D, window);
    Vec2_d tmp1 = myPCA(tmp, 3);
    Vec1_i label (H*W,0);
    Vec2_d tmp2 = tmp1;
    while(n > 9){
        label = Kmeans(tmp2, n, 3, init);
        tmp2 = ComputeNewMean(tmp1, label, window, H, W);
        --n;
    }
    res.push_back(label);
    DrawSegImg1(Im3D, label, window);
    //DrawSegImg(Im3D, label, window);
    return res;
}

Vec2_d GetTextureWin(C_Vec3_ui8& Im3D, C_i& window){
    Vec2_d res;
    int H (Dim2(Im3D)), W (Dim3(Im3D));
    Vec2_d core = genGaussianCore(window, HW4Sigma);
    Vec2_d core1 = genGaussianCore(Avg_win, HW4Sigma);
    Vec2_d tmp = toDouble(Im3D[0]);
    Vec2_d tmp1 (tmp);
    tmp1 = Conv2D_Ref(tmp, core, true);
    tmp = Add2D(tmp, tmp1, Sub_mean);
    map<string, Vec2_d> tmpF = TextureExtraction(tmp);
    Vec3_d fea = GetNeededEnergyMap(tmpF);
    /*
    for(int i(0); i < Dim1(fea); ++i){
        fea[i] = Conv2D_Ref(fea[i], core1, true);
    }*/
    NormalizeTextureLL(fea, fea[0]);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            Vec1_d t;
            for(int k(0); k < Dim1(fea); ++k){
                t.push_back(Sq(fea[k][i][j]));
            }
            res.push_back(t);
        }
    }
    res = Normalize2D_Col(res);
    return res;
}

Vec2_d GetTextureWin1(C_Vec3_ui8& Im3D, C_i& window){
    Vec2_d res;
    Vec2_d core = genGaussianCore(window, HW4Sigma);
    Vec2_d core1 = genGaussianCore(Avg_win, HW4Sigma);
    Vec2_d tmp = toDouble(Im3D[0]);
    Vec2_d tmp1 (tmp);
    tmp1 = Conv2D_Ref(tmp, core, true);
    tmp = Add2D(tmp, tmp1, Sub_mean);
    map<string, Vec2_d> tmpF = TextureExtraction(tmp);
    Vec3_d fea = GetNeededEnergyMap(tmpF);
    /*
    for(int i(0); i < Dim1(fea); ++i){
        Vec3_d t({fea[i]});
        t = NLMeansFilter(t, HW4Sigma, Search_win, Avg_win);
        fea[i] = t[0];
    }*/
    
    for(int i(0); i < Dim1(fea); ++i){
        fea[i] = Conv2D_Ref(fea[i], core1, true);
    }
    NormalizeTextureLL(fea, fea[0]);
    int H (Dim2(Im3D)), W (Dim3(Im3D));
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            Vec1_d t;
            for(int k(0); k < Dim1(fea); ++k){
                t.push_back(Sq(fea[k][i][j]));
            }
            res.push_back(t);
        }
    }
    int win2 = Sq(downsample_win);
    for(int i(0); i < H; i+= downsample_win){
        for(int j(0); j < W; j+=downsample_win){
            Vec1_d t;
            for(int k(0); k < Dim1(fea); ++k){
                double sum (0);
                for(int p(i); p < i+downsample_win; ++p){
                    for(int q(j); q < j+downsample_win; ++q){
                        sum += Sq(fea[k][p][q]);
                    }
                }
                t.push_back(sum/win2);
            }
            res.push_back(t);
        }
    }
    res = Normalize2D_Col(res);
    return res;
}

void DrawSegImg(C_Vec3_ui8& Im3D, C_Vec1_i& label, C_i& window){
    Vec2_ui8 tmp (Im3D[0]);
    int W (Dim3(Im3D));
    for(int i(0); i < Dim2(Im3D); ++i){
        for(int j(0); j < Dim3(Im3D); ++j){
            int lab = i*W+j;
            lab = label[lab];
            tmp[i][j] = 255-lab*40;
        }
    }
    Raw2Mat(tmp, str2pchar("Seg_comb_"+to_string(window)+"_"+to_string(clock())+".jpg"));
}

void DrawSegImg1(C_Vec3_ui8& Im3D, C_Vec1_i& label, C_i& window){
    Vec2_ui8 tmp (Im3D[0]), tmp1 (Im3D[0]), res (Im3D[0]);
    int H (Dim2(Im3D)), W (Dim3(Im3D)), S (H*W);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            int lab = i*W+j;
            lab = label[lab];
            tmp[i][j] = lab*40;
            //tmp[i][j] = lab;
        }
    }
    for(int i(0); i < H; i+=downsample_win){
        for(int j(0); j < W; j+=downsample_win){
            int lab = label[S];
            for(int p(i); p < i+downsample_win; ++p){
                for(int q(j); q < j+downsample_win; ++q){
                    tmp1[p][q] = lab*40;
                    //tmp1[p][q] = lab;
                }
            }
            ++S;
        }
    }
    for(int i(0); i < H; i++){
        for(int j(0); j < W; j++){
            res[i][j] = ChooseMajority(tmp, tmp1, i, j, mergelabel_win);
            //res[i][j] = ChooseMajorityGaussian(tmp, tmp1, i, j, mergelabel_win);
        }
    }
    /*
    Vec2_i hist = getHistogram({res});
    map<double, int> mp;
    for(int i(0); i < Dim2(hist); ++i){
        if(mp[hist[0][i]] == 0){
            mp[hist[0][i]] = i;
        }
        else{
            mp[hist[0][i]+0.1] = i;
        }
    }
    map<double, int>::reverse_iterator it = mp.rbegin();
    int i = 0;
    map<int, int> tt;
    while(it != mp.rend() && i < 7){
        tt[it->second] = i;
        ++it;
        ++i;
    }
    
    for(int i(0); i < H; i++){
        for(int j(0); j < W; j++){
            res[i][j] = 255-tt[res[i][j]]*40;
        }
    }*/
    Raw2Mat(res, str2pchar("Seg_comb_"+to_string(window)+"_"+to_string(clock())+".jpg"));
}

int ChooseMajority(C_Vec2_ui8& ImA, C_Vec2_ui8& ImB, C_i& i, C_i& j, C_i& r){
    map<int, int> tmp;
    for(int ii(max(0,i-r)); ii < min(Dim1(ImA),i+r); ++ii){
        for(int jj(max(0,j-r)); jj < min(Dim2(ImA),j+r); ++jj){
            ++tmp[ImA[ii][jj]];
            ++tmp[ImB[ii][jj]];
        }
    }
    map<int,int>::iterator it = tmp.begin();
    int m (INT_MIN),mm (0);
    while(it != tmp.end()){
        if(m < it->second){
            m = it->second;
            mm = it->first;
        }
        ++it;
    }
    return mm;
}

int ChooseMajorityGaussian(C_Vec2_ui8& ImA, C_Vec2_ui8& ImB, C_i& i, C_i& j, C_i& r){
    map<int, double> tmp;
    for(int ii(max(0,i-r)); ii < min(Dim1(ImA),i+r); ++ii){
        for(int jj(max(0,j-r)); jj < min(Dim2(ImA),j+r); ++jj){
            tmp[ImA[ii][jj]] += genGaussianCe(HW4Sigma, i, j, ii, jj, 0);
            tmp[ImB[ii][jj]] += genGaussianCe(HW4Sigma, i, j, ii, jj, 0);
        }
    }
    map<int,double>::iterator it = tmp.begin();
    int m (INT_MIN),mm (0);
    while(it != tmp.end()){
        if(m < it->second){
            m = it->second;
            mm = it->first;
        }
        ++it;
    }
    return mm;
}

Vec2_d ComputeNewMean(C_Vec2_d& fea, C_Vec1_i& label, C_i& win, C_i& H, C_i& W){
    Vec2_d tmp;
omp_set_num_threads(4);
#pragma omp parallel for
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double sum(0), ct(0.000001);
            int index = i*W+j;
            Vec1_d tfea;
            for(int k(0); k < Dim2(fea); ++k){
                for(int ii = max(0, i-win); ii < min(H, i+win); ++ii){
                    for(int jj = max(0, j-win); jj < min(W, j+win); ++jj){
                        int indext = ii*W+jj;
                        if(label[index] == label[indext]){
                            sum += fea[indext][k];
                            ct += 1;
                        }
                    }
                }
                tfea.push_back(sum/ct);
            }
            tmp.push_back(tfea);
        }
    }
    for(int i(0); i < H; i+=downsample_win){
        for(int j(0); j < W; j+=downsample_win){
            double sum(0), ct(0.000001);
            Vec1_d tfea;
            for(int k(0); k < Dim2(fea); ++k){
                for(int ii = max(0, i-downsample_win); ii < min(H, i+downsample_win); ++ii){
                    for(int jj = max(0, j-downsample_win); jj < min(W, j+downsample_win); ++jj){
                        int indext = ii*W+jj;
                        sum += fea[indext][k];
                        ct += 1;
                    }
                }
                tfea.push_back(sum/ct);
            }
            tmp.push_back(tfea);
        }
    }
    return tmp;
}

Vec1_i LargestKCluster(Vec1_i& label, C_i& oK, C_i& K){
    Vec1_i bin (oK, 0);
    Vec1_i res;
    for(int i(0); i < Dim1(label); ++i){
        ++bin[label[i]];
    }
    map<double, int> mp;
    for(int i(0); i < oK; ++i){
        if(mp[bin[i]] == 0){
            mp[bin[i]] = i;
        }
        else{
            mp[bin[i]+0.1] = i;
        }
    }
    map<double, int>::reverse_iterator it = mp.rbegin();
    int i = 0;
    while(it != mp.rend() && i < K){
        res.push_back(it->second);
        ++it;
        ++i;
    }
    return res;
}
