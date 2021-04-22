//
//  MorphologicalProcessing.cpp
//  EE569
//
//  Created by Alex on 2019/2/12.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "MorphologicalProcessing.hpp"
#include "Template.hpp"

Vec2_ui8 Bridging(C_Vec2_ui8& Im2D){
    Vec2_ui8 res (Im2D);
    for(int i(1); i < Dim1(Im2D)-1; ++i){
        for(int j(1); j < Dim2(Im2D)-1; ++j){
            if(Im2D[i][j] != 0){
                continue;
            }
            bool x = Im2D[i][j] != 0;
            bool x0 = Im2D[i][j+1] != 0;
            bool x1 = Im2D[i-1][j+1] != 0;
            bool x2 = Im2D[i-1][j] != 0;
            bool x3 = Im2D[i-1][j-1] != 0;
            bool x4 = Im2D[i][j-1] != 0;
            bool x5 = Im2D[i+1][j-1] != 0;
            bool x6 = Im2D[i+1][j] != 0;
            bool x7 = Im2D[i+1][j+1] != 0;
            
            bool L1 = !x & !x0 & x1 & !x2 & x3 & !x4 & !x5 & !x6 & !x7;
            bool L2 = !x & !x0 & !x1 & !x2 & x3 & !x4 & x5 & !x6 & !x7;
            bool L3 = !x & !x0 & !x1 & !x2 & !x3 & !x4 & x5 & !x6 & x7;
            bool L4 = !x & !x0 & x1 & !x2 & !x3 & !x4 & !x5 & !x6 & x7;
            bool Pq = L1 | L2 | L3 | L4;
            bool P1 = !x2 & !x6 & (x3 | x4 | x5) & (x0 | x1 | x7) &!Pq;
            bool P2 = !x0 & !x4 & (x1 | x2 | x3) & (x5 | x6 | x7) &!Pq;
            bool P3 = !x0 & !x6 & x7 & (x2 | x3 | x4);
            bool P4 = !x0 & !x2 & x1 & (x4 | x5 | x6);
            bool P5 = !x2 & !x4 & x3 & (x0 | x6 | x7);
            bool P6 = !x4 & !x6 & x5 & (x0 | x1 | x2);
            bool P = x | P1 | P2 | P3 | P4 | P5 | P6;
            if(P == 1){
                res[i][j] = 255;
            }
        }
    }
    return res;
}

Vec2_ui8 Morphological2D(C_Vec2_ui8& Im2D, BinaryDict* Cond, BinaryDict* UnCond, bool& modify, C_i& Opt){
    int H (Dim1(Im2D));
    int W (Dim2(Im2D));
    Vec2_ui8 tmp init2_ui8(H, W, 0);
    Vec2_ui8 res (Im2D);
    for(int i(0); i < H ; ++i){
        for(int j(0); j < W ; ++j){
            if(Im2D[i][j] == 0){
                continue;
            }
            if(SearchBinaryDict(Cond, Im2D, i, j, Opt, H, W) == 1){
                tmp[i][j] = 255;
            }
        }
    }
    for(int i(0); i < H ; ++i){
        for(int j(0); j < W ; ++j){
            if(tmp[i][j] == 0){
                continue;
            }
            if(SearchBinaryDict(UnCond, tmp, i, j, Opt, H, W) == 0){
                res[i][j] = 0;
                modify = true;
            }
        }
    }
    return res;
}

Vec3_ui8 MorphologicalProcessing(C_Vec3_ui8& Im3D, C_i& Opt, C_i& maxiter){
    BinaryDict* head = BuildBinaryDict(Cond_Mask, Cond_Label);
    BinaryDict* head1;
    if(Opt == 2){
        head1 = BuildBinaryDict(UnCond_MaskK, UnCond_LabelK);
    }
    else{
        head1 = BuildBinaryDict(UnCond_MaskST, UnCond_LabelST);
    }
    Vec2_ui8 tmp (Im3D[0]);
    bool modify (0);
    int ct (0);
    while(ct < maxiter){
        tmp = Morphological2D(tmp, head, head1, modify, Opt);
        if(modify != true){
            break;
        }
        modify = false;
        ++ct;
    }
    Vec3_ui8 res;
    res.push_back(tmp);
    if(Opt == 2){
        res[0] = Bridging(res[0]);
    }
    return res;
}

int DFS(Vec2_ui8& mat2d, C_i& i, C_i& j, Vec2_i& position){
    if(i < 0 || i >= Dim1(mat2d) || j < 0 || j >= Dim2(mat2d) || mat2d[i][j] !=0){
        return 0;
    }
    mat2d[i][j] = 1;
    position.push_back({i,j});
    return 1+DFS(mat2d, i-1, j, position)+DFS(mat2d, i-1, j-1, position)+DFS(mat2d, i-1, j+1, position)+DFS(mat2d, i+1, j-1, position)+DFS(mat2d, i+1, j, position)+DFS(mat2d, i+1, j+1, position)+DFS(mat2d, i, j-1,position)+DFS(mat2d, i, j+1, position);
}

Vec3_i SearchEdges(C_Vec2_ui8& mat2d, C_i& threshold, C_b& flag){
    Vec2_ui8 tmp (mat2d);
    Vec3_i loc;
    Vec2_i pos;
    for(int i(0); i < Dim1(mat2d); ++i){
        for(int j(0); j < Dim2(mat2d); ++j){
            if(tmp[i][j] == 0){
                int t = DFS(tmp, i, j, pos);
                if(t > threshold && flag == 1){
                    loc.push_back(pos);
                }
                else if(t < threshold && flag == 0){
                    loc.push_back(pos);
                }
                pos.clear();
            }
        }
    }
    return loc;
}

Vec2_ui8 MaskSearch(C_Vec2_ui8& Im2D, BinaryDict* head){
    int H (Dim1(Im2D)), W (Dim2(Im2D));
    Vec2_ui8 tmp(Im2D);
    for(int i(0); i < H ; ++i){
        for(int j(0); j < W ; ++j){
            if(Im2D[i][j] != 0){
                continue;
            }
            if(SearchBinaryDict(head, Im2D, i, j, 0, H, W) == 1){
                tmp[i][j] = 255;
            }
        }
    }
    return tmp;
}

Vec3_ui8 DefectDetect(C_Vec3_ui8& Im3D){
    Vec2_ui8 Im2D (Im3D[0]);
    Vec1_s mask ({"000000000"});
    Vec1_s label ({"x"});
    BinaryDict* head = BuildBinaryDict(mask, label);
    Vec2_ui8 tmp(MaskSearch(Im2D, head));
    Vec3_ui8 res(Im3D);
    Vec3_i loc = SearchEdges(tmp,50, 0);
    cout<<"DefectDetect: There are "<<Dim1(loc)<<" defects"<<endl;
    DrawDefect(Im3D, loc);
    res = ClearDefect(Im3D, loc);
    return res;
}

Vec3_ui8 ClearDefect(C_Vec3_ui8& Im3D, C_Vec3_i& loc){
    Vec3_ui8 res;
    res.push_back(Im3D[0]);
    for(int k(0); k < Dim1(loc); ++k){
        for(int i(0); i < Dim1(loc[k]); ++i){
            res[0][loc[k][i][0]][loc[k][i][1]] = 255;
        }
    }
    return res;
}

void DrawDefect(C_Vec3_ui8& Im3D, C_Vec3_i& loc){
    Vec3_ui8 res;
    res.push_back(Im3D[0]);
    res.push_back(Im3D[0]);
    res.push_back(Im3D[0]);
    for(int k(0); k < Dim1(loc); ++k){
        for(int i(0); i < Dim1(loc[k]); ++i){
            res[0][loc[k][i][0]][loc[k][i][1]] = 0;
            res[1][loc[k][i][0]][loc[k][i][1]] = 0;
            res[2][loc[k][i][0]][loc[k][i][1]] = 255;
        }
    }
    WriteRaw("defect_labeled.raw", res);
}

Vec2_ui8 RemoveBackGround(C_Vec2_ui8& Im2D){
    int H (Dim1(Im2D)), W (Dim2(Im2D));
//    int t = OtsuT(Im2D);
    int t = Mode2D(Im2D);
    Vec2_ui8 res init2_ui8(H, W, 0);
    for(int i(0); i < H ; ++i){
        for(int j(0); j < W ; ++j){
            res[i][j] = abs(Im2D[i][j] -t)< 3 ? 0 : 1;
        }
    }
    return res;
}

Vec2_ui8 RemoveNoise(C_Vec2_ui8& Im2D, C_i& threshold){
    int H (Dim1(Im2D)), W (Dim2(Im2D));
    Vec2_ui8 res (Im2D);
    for(int i(0); i < H ; ++i){
        for(int j(0); j < W ; ++j){
            if(i > 0 && i < H-1 && j > 0 && j < W-1){
                int score = Im2D[i-1][j-1]+Im2D[i-1][j+1]+Im2D[i+1][j-1]+Im2D[i+1][j+1]+2*(Im2D[i+1][j]+Im2D[i-1][j]+Im2D[i][j-1]+Im2D[i][j+1]);
                res[i][j] = score <= threshold ? 255 : 0;
            }
            else{
                res[i][j] = 255;
            }
        }
    }
    return res;
}

Vec3_ui8 RiceProcess1(C_Vec3_ui8& Im3D){
    Vec3_ui8 tmp = RGB2Gray(tmp);
    tmp = NLMeansFilter(tmp, 10, 7, 3);
    tmp[0] = RemoveBackGround(tmp[0]);
    tmp[0] = RemoveNoise(tmp[0], 7);
    return tmp;
}

int RiceCount(C_Vec3_ui8& Im3D){
    Vec3_ui8 tmp = RiceProcess1(Im3D);
    Vec3_i loc = SearchEdges(tmp[0], 10, 1);
    int count (0);
    for(int i(0); i < Dim1(loc); ++i){
        if(Dim1(loc[i]) > 10){
            ++count;
        }
    }
    cout<<"RiceCount: There are "<<count<<" rice."<<endl;
    WriteRaw("rice_count.raw", tmp);
    return count;
}

void FillHole(Vec3_ui8& Im3D, Vec3_i& loc){
    for(int i(0); i < Dim1(loc); ++i){
        map<int, vector<int>> mp, mp1;
        for(int j(0); j < Dim1(loc[i])-1; ++j){
            mp[loc[i][j][0]].push_back(loc[i][j][1]);
        }
        map<int, vector<int>>::iterator iter = mp.begin();
        while(iter != mp.end()){
            sort(iter->second.begin(), iter->second.end());
            for(int j(0); j < Dim1(iter->second)-1; ++j){
                if(iter->second[j] != iter->second[j+1]-1){
                    for(int k(iter->second[j]+1); k < iter->second[j+1]; ++k){
                        Im3D[0][iter->first][k] = 0;
                        loc[i].push_back({iter->first, k});
                    }
                }
            }
            ++iter;
        }
        for(int j(0); j < Dim1(loc[i])-1; ++j){
            mp1[loc[i][j][1]].push_back(loc[i][j][0]);
        }
        iter = mp1.begin();
        while(iter != mp1.end()){
            sort(iter->second.begin(), iter->second.end());
            for(int j(0); j < Dim1(iter->second)-1; ++j){
                if(iter->second[j] != iter->second[j+1]-1){
                    for(int k(iter->second[j]+1); k < iter->second[j+1]; ++k){
                        Im3D[0][k][iter->first] = 0;
                        loc[i].push_back({ k, iter->first});
                    }
                }
            }
            ++iter;
        }
    }
}

Vec2_d GetRiceCenter(C_Vec3_i& loc){
    Vec2_d res;
    for(int i(0); i < Dim1(loc); ++i){
        double x (0), y (0), area (Dim1(loc[i]));
        for(int j(0); j < area; ++j){
            x += loc[i][j][0];
            y += loc[i][j][1];
        }
        x /= area;
        y /= area;
        res.push_back({x,y,area});
    }
    return res;
}

void RiceSize(C_Vec3_ui8& Im3D, C_i& Nclass){
    Vec3_ui8 tmp (RiceProcess1(Im3D));
    Vec3_i loc (SearchEdges(tmp[0], 10, 1));
    FillHole(tmp, loc);
    Vec2_d cloc (GetRiceCenter(loc));
    Vec2_d tcloc;
    for(int i(0); i < Dim1(cloc); ++i){
        tcloc.push_back({cloc[i][0]/500,cloc[i][1]/690});
    }
    Vec1_i labels (Kmeans(tcloc, Nclass, 2, 1));
    DrawClassifiedRice(tmp[0], cloc, labels, Nclass);
    
    Vec2_d avgArea init2_d(Nclass, 4, 0);
    for(int i(0); i < Dim1(cloc); ++i){
        avgArea[labels[i]][0] += cloc[i][2];
        avgArea[labels[i]][1] += 1;
        avgArea[labels[i]][2] += cloc[i][0];
        avgArea[labels[i]][3] += cloc[i][1];
    }
    map<double, int> mp;
    for(int j(0); j < Nclass; ++j){
        avgArea[j][0] /= avgArea[j][1];
        avgArea[j][2] /= avgArea[j][1];
        avgArea[j][3] /= avgArea[j][1];
        mp[avgArea[j][0]] = j;
    }
    
    cout<<"RiceSize:"<<endl;
    map<int, int> mpp;
    map<double, int>::iterator iter = mp.begin();
    int ii (0);
    while(iter != mp.end()){
        cout<<"   label: "<<iter->second<<",  size: "<<iter->first<<endl;
        mpp[iter->second] = ii;
        ++iter;
        ++ii;
    }
    DrawSortedRice(tmp, cloc, labels, mpp, Nclass);
}

void DrawClassifiedRice(C_Vec2_ui8& Im2D, C_Vec2_d& cloc, C_Vec1_i& labels, C_i& Nclass){
    Vec3_ui8 tmp;
    tmp.push_back(Im2D);
    for(int i(0); i < Dim1(cloc); ++i){
        DrawDFS(tmp[0], cloc[i][0], cloc[i][1], labels[i], Nclass);
     }
    DrawBar(tmp, Nclass);
    WriteRaw("rice_classified.raw", tmp);
}

void DrawSortedRice(Vec3_ui8& Im3D, C_Vec2_d& cloc, C_Vec1_i& labels, map<int, int> mpp, C_i& Nclass){
    for(int i(0); i < Dim1(cloc); ++i){
        DrawDFS(Im3D[0], (int)cloc[i][0], (int)cloc[i][1], mpp[labels[i]], Nclass);
    }
    DrawBar(Im3D, Nclass);
    WriteRaw("rice_sorted.raw", Im3D);
}

void DrawBar(Vec3_ui8& Im3D, C_i& Nclass){
    Vec1_ui8 ttt (Dim3((Im3D)), 255);
    for(int j(0); j < Dim3(Im3D); ++j){
        int t = j*Nclass/(Dim3(Im3D));
        ttt[j] = 255/Nclass*t+20;
    }
    for(int i(0); i < 40; ++i){
        Im3D[0].push_back(ttt);
    }
}

void DrawDFS(Vec2_ui8& mat2d, C_i& i, C_i& j, C_i& labels, C_i& Nclass){
    if(i < 0 || i >= Dim1(mat2d) || j < 0 || j >= Dim2(mat2d) || mat2d[i][j] !=0){
        return;}
    mat2d[i][j] = 255/Nclass*labels+20;
    DrawDFS(mat2d, i-1, j, labels, Nclass);
    DrawDFS(mat2d, i-1, j-1, labels, Nclass);
    DrawDFS(mat2d, i-1, j+1, labels, Nclass);
    DrawDFS(mat2d, i+1, j-1, labels, Nclass);
    DrawDFS(mat2d, i+1, j, labels, Nclass);
    DrawDFS(mat2d, i+1, j+1, labels, Nclass);
    DrawDFS(mat2d, i, j-1,labels, Nclass);
    DrawDFS(mat2d, i, j+1, labels, Nclass);
}
