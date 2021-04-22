//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  HW3Fancy.cpp
//  EE569
//
//  Created by Alex on 2019/2/13.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "HW3Fancy.hpp"

void DeerFancy(){
    Vec3_ui8 Img3D;
    Img3D = ReadRaw("deer.raw", 1, 691, 550);
    Img3D = toUint(ImgOpposition(Img3D));
    Img3D = MorphologicalProcessing(Img3D, 0, 5);
    WriteRaw("deer_facy.raw", Img3D);
}

void RiceSizeFancy(C_Vec3_ui8& Im3D, C_i& Nclass){
    Vec3_ui8 tIm (GaussianFilter(Im3D, 5, 200));
    Vec3_ui8 tmp (RiceProcess1(Im3D));
    Vec3_i loc (SearchEdges(tmp[0], 10, 1));
    FillHole(tmp, loc);
    Vec2_d cloc (GetRiceCenter(loc));
    Vec3_d tImg (RGB2Lab(tIm));
    Vec2_d color;
    Vec1_d t = calRiceLength(loc);
    for(int ii(0); ii < Dim1(cloc); ++ii){
        Vec1_d sum (5, 0);
        int i (cloc[ii][0]), j (cloc[ii][1]);
        sum[0] = tImg[0][i][j];
        sum[1] = tImg[1][i][j];
        sum[2] = tImg[2][i][j];
        sum[3] = t[i]*24;
        sum[5] = Dim1(loc[ii])/t[i];
        color.push_back(sum);
    }
    Vec1_i label (Dim1(cloc),0);
    label = Kmeans(color, Nclass, 4, 1);
    DrawClassifiedRice(tmp[0], cloc, label, Nclass);
}

Vec1_d calRiceLength(C_Vec3_i& loc){
    Vec1_d res;
    for(int i(0); i < Dim1(loc); ++i){
        double length(0);
        int P(Dim1(loc[i]));
        for(int j(0); j < P; ++j){
            for(int k(j+1); k < P; ++k){
                double tmp = sqrt(Sq(loc[i][j][0]-loc[i][k][0])+Sq(loc[i][j][1]-loc[i][k][1]));
                length = max(length, tmp);
            }
        }
        res.push_back(length);
    }
    return res;
}

Vec2_d KmeansInit1(C_Vec2_d& data, C_i& N, C_i& dim){
    Vec2_d cent;
    map<int,int> mp;
    cent.push_back(data[0]);
    mp[0] =1;
    for(int i(1); i < N; ++i){
        double mxd(0), index(0);
        for(int j(0); j < Dim1(data); ++j){
            double dist (0);
            for(int k(0); k < Dim1(cent); ++k){
                for(int p(0); p < dim; ++p){
                    dist += Sq(cent[k][p]-data[j][p])*exp((-Sq(data[i][3])-Sq(data[i][4]))/100);
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

void KmeansCluster1(C_Vec2_d& data, Vec2_d& center, Vec1_i& label, C_i& dim, bool& modified){
    map<int, Vec2_d> tmp;
    Vec2_d newCent init2_d(Dim1(center), dim+1, 0);
    for(int i(0); i < Dim1(data); ++i){
        double mindist = INT_MAX;
        int lab (0);
        for(int j(0); j < Dim1(center); ++j){
            double dist(0);
            for(int ii(0); ii < dim; ++ii){
                dist += Sq(data[i][ii]-center[j][ii])*exp((-Sq(data[i][3])-Sq(data[i][4]))/100);
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

Vec1_i Kmeans1(C_Vec2_d& loc, C_i& N, C_i& dim){
    Vec2_d center = KmeansInit1(loc, N, dim);
    Vec1_i label (Dim1(loc), 1);
    bool modify = true;
    while(1){
        modify = false;
        KmeansCluster1(loc, center, label, dim, modify);
        if(modify == false){
            break;
        }
    }
    return label;
}

Vec2_ui8 SpatialWarping2DBack(C_Vec2_ui8& Im2D, C_Vec2_d& coef){
    double xx = 0.053;
    Vec2_d tmp = {
        {0,1,0,-xx,0,xx},{0,0,1,0,0,0},
        {0,1,0,0,0,0},{0,0,1,-xx,0,xx},
        {0,1,0,xx,0,-xx},{0,0,1.0000,0,0,0},
        {0,1,0,0,0,0},{0,0,1,xx,0,-xx}};
    int H (Dim1(Im2D)), W (Dim2(Im2D));
    int HH (H>>1), WW (W>>1);
    Vec2_ui8 res init2_ui8(Dim1(Im2D),Dim2(Im2D),0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double nx (0), ny (0);
            double ii (i-HH), jj (j-WW);
            double iii (i-HH), jjj (j-WW);
            ii /= 256;
            jj /= 256;
            for (int j(0); j < 20; j++){
                if(ii < 0 && abs(jjj) < abs(iii)){
                    nx = coef[0][0] + coef[0][1]*ii + coef[0][2]*jj + coef[0][3]*Sq(ii) + coef[0][4]*ii*jj+coef[0][5]*Sq(jj);
                    ny = coef[1][0] + coef[1][1]*ii + coef[1][2]*jj + coef[1][3]*Sq(ii) + coef[1][4]*ii*jj+coef[1][5]*Sq(jj);
                }
                if(jj > 0 && abs(iii) < abs(jjj)){
                    nx = coef[2][0] + coef[2][1]*ii + coef[2][2]*jj + coef[2][3]*Sq(ii) + coef[2][4]*ii*jj+coef[2][5]*Sq(jj);
                    ny = coef[3][0] + coef[3][1]*ii + coef[3][2]*jj + coef[3][3]*Sq(ii) + coef[3][4]*ii*jj+coef[3][5]*Sq(jj);
                }
                if(ii > 0 && abs(jjj) < abs(iii)){
                    nx = coef[4][0] + coef[4][1]*ii + coef[4][2]*jj + coef[4][3]*Sq(ii) + coef[4][4]*ii*jj+coef[4][5]*Sq(jj);
                    ny = coef[5][0] + coef[5][1]*ii + coef[5][2]*jj + coef[5][3]*Sq(ii) + coef[5][4]*ii*jj+coef[5][5]*Sq(jj);
                }
                if(jj < 0 && abs(iii) < abs(jjj)){
                    nx = coef[6][0] + coef[6][1]*ii + coef[6][2]*jj + coef[6][3]*Sq(ii) + coef[6][4]*ii*jj+coef[6][5]*Sq(jj);
                    ny = coef[7][0] + coef[7][1]*ii + coef[7][2]*jj + coef[7][3]*Sq(ii) + coef[7][4]*ii*jj+coef[7][5]*Sq(jj);
                }
                ii = nx;
                jj = ny;
            }
            nx = Max(0, Min(nx*256+HH,H-1));
            ny = Max(0, Min(ny*256+WW,W-1));
            int xx (Min(H-1, Max(0,nx)));
            int yy (Min(W-1, Max(0,ny)));
            res[xx][yy] = Im2D[i][j];
        }
    }
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            if(abs(i-HH) == abs(j-WW)){
                res[i][j] = Im2D[i][j];}
        }
    }
    return res;
}

//x, y, x_d, y_d
Vec2_d GetTriplets(C_i& H, C_i& W){
    Vec2_d res;
    ofstream myfile;
    myfile.open ("example.txt");
    int HH = H>>1, WW = W>>1;
    for(int i(0); i < H; i+=70){
        for(int j(0); j < W; j+=40){
            double ii((i-HH)), jj((j-WW));
            ii /= 600;
            jj /= 600;
            double r = Sq(ii)+Sq(jj);
            double R = 1-0.3536*r+0.1730*Sq(r);
            double iiR = ii*R;
            double jjR = jj*R;
            res.push_back({(double)ii*ii,(double)jj*jj,iiR, jjR});
            myfile<<r<<endl;
            myfile<<Sq(r)<<endl;
            myfile<<r*r*r<<endl;
            myfile<<Sq(iiR)+Sq(jjR)<<endl;
        }
    }
    myfile.close();
    return res;
}

Vec2_ui8 T(C_Vec2_ui8& Im2D){
    int H (Dim1(Im2D)), W (Dim2(Im2D));
    int HH (H>>1), WW (W>>1);
    Vec2_ui8 res init2_ui8(H, W, 0);
    for(int i(0); i < HH; ++i){
        for(int j(0); j < WW; ++j){
            double x ((i-HH)), y ((j-WW));
            x = x/600;
            y = y/600;
            if(x == 0 || y == 0){continue;}
            double rd = Sq(x)+Sq(y);
            double r = 1.08525813*rd;
            double nx;
            double ny ;
            ny = -sqrt(r/(1+Sq(x/y)));
            nx = -sqrt(r/(1+Sq(y/x)));
            double cx = (nx*600+HH);
            double cy = (ny*600+WW);
            int xx (Min(H-1, Max(0,cx)));
            int yy (Min(W-1, Max(0,cy)));
            res[xx][yy] = Im2D[i][j];
        }
    }
    return res;
}

