//  Yifan Wang
//  3038184983
//  wang608@usc.edu
//
//  Matrix.hpp
//  EE569
//
//  Created by Alex on 2019/1/15.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include "Basic.hpp"
using namespace std;

template<class T>
double Sum2D(const Vec2_T& mat2D){
    double sum (0);
    for(int i(0); i < Dim1(mat2D); ++i){
        for(int j(0); j < Dim2(mat2D); ++j){
            sum += mat2D[i][j];
        }
    }
    return sum;
}

template<class T>
double AbsSum2D(const Vec2_T& mat2D, C_i bound){
    double sum (0);
    for(int i(bound); i < Dim1(mat2D)-bound; ++i){
        for(int j(bound); j < Dim2(mat2D)-bound; ++j){
            sum += abs(mat2D[i][j]);
        }
    }
    return sum;
}

template<class T>
double Max2D(const Vec2_T& mat2D){
    double m (INT_MIN);
    for(int i(0); i < Dim1(mat2D); ++i){
        for(int j(0); j < Dim2(mat2D); ++j){
            m = max(m, mat2D[i][j]);
        }
    }
    return m;
}

template<class T>
double Min2D(const Vec2_T& mat2D){
    double m (INT_MAX);
    for(int i(0); i < Dim1(mat2D); ++i){
        for(int j(0); j < Dim2(mat2D); ++j){
            m = min(m, mat2D[i][j]);
        }
    }
    return m;
}

template<class T>
T Mode2D(const Vec2_T& mat2D){
    map<T, int> mp;
    int mx (INT_MIN);
    T idx (0);
    for(int i(0); i < Dim1(mat2D); ++i){
        for(int j(0); j < Dim2(mat2D); ++j){
            ++mp[mat2D[i][j]];
            if(mx < mp[mat2D[i][j]]){
                idx = mat2D[i][j];
                mx = mp[mat2D[i][j]];
            }
        }
    }
    return idx;
}

template<class T>
double SqSum2D(const Vec2_T& mat2D, C_i bound){
    double sum (0);
    for(int i(bound); i < Dim1(mat2D)-bound; ++i){
        for(int j(bound); j < Dim2(mat2D)-bound; ++j){
            sum += Sq(mat2D[i][j]);
        }
    }
    return sum;
}

template<class T>
Vec2_d Sq2D(const Vec2_T& mat2D){
    Vec2_d res init2_d(Dim1(mat2D), Dim2(mat2D), 0);
    for(int i(0); i < Dim1(mat2D); ++i){
        for(int j(0); j < Dim2(mat2D); ++j){
            res[i][j] = Sq(mat2D[i][j]);
        }
    }
    return res;
}

template<class T>
Vec2_d Abs2D(const Vec2_T& mat2D){
    Vec2_d res init2_d(Dim1(mat2D), Dim2(mat2D), 0);
    for(int i(0); i < Dim1(mat2D); ++i){
        for(int j(0); j < Dim2(mat2D); ++j){
            res[i][j] = abs(mat2D[i][j]);
        }
    }
    return res;
}

template<class T>
double Normalize2D(Vec2_T& mat2D){
    double sum (Sum2D(mat2D));
    if(sum == 0){return 1;}
    for(int i(0); i < Dim1(mat2D); ++i){
        for(int j(0); j < Dim2(mat2D); ++j){
            mat2D[i][j] /= sum;
        }
    }
    return sum;
}

template<class T>
Vec2_T Normalize2D_2Vui8(const Vec2_T& mat2D){
    double m1 (Max2D(mat2D)), m2 (Min2D(mat2D));
    Vec2_T res init2_T(Dim1(mat2D), Dim2(mat2D), 0);
    for(int i(0); i < Dim1(mat2D); ++i){
        for(int j(0); j < Dim2(mat2D); ++j){
            res[i][j] = 255*(mat2D[i][j]-m2)/(m1-m2+1);
        }
    }
    return res;
}

template<class T>
Vec2_T Normalize2D_Col(const Vec2_T& mat2D){
    Vec2_T res init2_T(Dim1(mat2D), Dim2(mat2D), 0);
    for(int j(0); j < Dim2(mat2D); ++j){
        double m1 (INT_MIN), m2 (INT_MAX);
        for(int i(0); i < Dim1(mat2D); ++i){
            m1 = mat2D[i][j] > m1 ? mat2D[i][j] : m1;
            m2 = mat2D[i][j] < m2 ? mat2D[i][j] : m2;
        }
        for(int i(0); i < Dim1(mat2D); ++i){
            res[i][j] = 255*(mat2D[i][j]-m2)/(m1-m2+0.00001);
        }
    }
    return res;
}

//refect bound
template<class T>
void RefectBound(T& ii, T& jj, C_i& H, C_i& W){
    T iii (ii), jjj (jj);
    if(ii < 0){iii = -ii-1;}
    if(jj < 0){jjj = -jj-1;}
    if(ii >= H){iii = 2*H-ii-1;}
    if(jj >= W){jjj = 2*W-jj-1;}
    ii = iii;
    jj = jjj;
}

//Conv
template<class T>
Vec2_d Conv(const Vec2_T& mat2D_1, Vec2_d& mat2D_2, C_i& pooling, C_i& stride, C_b& normalize){
    int H (Dim1(mat2D_1)), W (Dim2(mat2D_1)), F (Dim1(mat2D_2));
    if(normalize == true){
        Normalize2D(mat2D_2);
        
    }
    Vec2_T res init2_T((H-F+2*pooling)/stride+1, (W-F+2*pooling)/stride+1, 0);
    int iii(0), jjj(0);
    for(int i(0); i < H; i+=stride){
        for(int j(0); j < W; j+=stride){
            double sum (0);
            for(int ii(max(i-pooling, 0)); ii < min(i+pooling+1, H); ++ii){
                for(int jj(max(j-pooling, 0)); jj < min(j+pooling+1, W); ++jj){
                    sum += mat2D_1[ii][jj] * mat2D_2[ii-i+pooling][jj-j+pooling];
                }
            }
            res[iii][jjj] = sum;
            ++jjj;
        }
        ++iii;
        jjj = 0;
    }
    return res;
}

//same conv zero padding
template<class T>
Vec2_d Conv2D(const Vec2_T& mat2D_1, Vec2_d& mat2D_2, C_b& normalize){
    int H (Dim1(mat2D_1)),  W (Dim2(mat2D_1)), n (Dim1(mat2D_2)>>1);
    if(normalize == true){
        Normalize2D(mat2D_2);
    }
    Vec2_d res init2_d(H, W, 0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double sum(0);
            for(int ii(max(i-n, 0)); ii < min(i+n+1, H); ++ii){
                for(int jj(max(j-n, 0)); jj < min(j+n+1, W); ++jj){
                    sum += mat2D_1[ii][jj] * mat2D_2[ii-i+n][jj-j+n];
                }
            }
            res[i][j] = sum;
        }
    }
    return res;
}

//same conv reflection padding
template<class T>
Vec2_d Conv2D_Ref(const Vec2_T& mat2D_1, Vec2_d& mat2D_2, C_b& normalize){
    int H (Dim1(mat2D_1)),  W (Dim2(mat2D_1)), n (Dim1(mat2D_2)>>1);
    if(normalize == true){
        Normalize2D(mat2D_2);
    }
    Vec2_d res init2_d(H, W, 0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            double sum (0);
            for(int ii(i-n); ii < i+n+1; ++ii){
                for(int jj(j-n); jj < j+n+1; ++jj){
                    int iii (ii), jjj (jj);
                    RefectBound(iii, jjj, H, W);
                    sum += mat2D_1[iii][jjj] * mat2D_2[ii-i+n][jj-j+n];
                }
            }
            res[i][j] = sum;
        }
    }
    return res;
}

//operation != 0: mat1 + flag*mat2
//operation = 0: abs(mat1-mat2)
template<class T, class T1>
Vec2_d Add2D(const Vec2_T& mat2D_1, const vector<vector<T1>>& mat2D_2, C_d& operation){
    Vec2_d res init2_d(Dim1(mat2D_1), Dim2(mat2D_1), 0);
    for(int i(0); i < Dim1(mat2D_1); ++i){
        for(int j(0); j < Dim2(mat2D_1); ++j){
            if(operation != 0){
                res[i][j] = mat2D_1[i][j]+operation*mat2D_2[i][j];
            }
            else{
                double t (mat2D_1[i][j]-mat2D_2[i][j]);
                res[i][j] = max(t, -t);
            }
        }
    }
    return res;
}

template<class T, class T1>
Vec2_d Add2D_1(const Vec2_T& mat2D_1, const T1& num, C_d& operation){
    Vec2_d res init2_d(Dim1(mat2D_1), Dim2(mat2D_1), 0);
    for(int i(0); i < Dim1(mat2D_1); ++i){
        for(int j(0); j < Dim2(mat2D_1); ++j){
            if(operation != 0){
                res[i][j] = mat2D_1[i][j]+operation*num;
            }
            else{
                double t (mat2D_1[i][j]-num);
                res[i][j] = max(t, -t);
            }
        }
    }
    return res;
}

//Max pooling 2*2
template<class T>
Vec2_T MaxPooling2D(const Vec2_T& mat2D){
    int H (Dim1(mat2D)), W (Dim2(mat2D));
    Vec2_T Res init2_T(H/2, W/2, 0);
    for(int i(0); i < H; i+=2){
        for(int j(0); j < W; j+=2){
            uint8_t t1(max(mat2D[i][j], mat2D[i][j+1]));
            Res[i>>1][j>>1] = max(mat2D[i+1][j+1], max(t1, mat2D[i+1][j]));
        }
    }
    return Res;
}

//Roate 2D mat clockwise 90 degree
template<class T>
Vec2_T RotateCW90(const Vec2_T& mat2D){
    int H (Dim1(mat2D));
    Vec2_T res init2_T(Dim2(mat2D), H, 0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < Dim2(mat2D); ++j){
            res[j][H-i-1] = mat2D[i][j];
        }
    }
    return res;
}

//Tensor product, mat2D_1 x mat2D_2
template<class T, class T1>
Vec2_T TensorProduct2D(const Vec2_T& mat2D_1, const vector<vector<T1>>& mat2D_2){
    int H1 (Dim1(mat2D_1)), W1 (Dim2(mat2D_1));
    int H2 (Dim1(mat2D_2)), W2 (Dim2(mat2D_2));
    int H (H1*H2), W (W1*W2);
    Vec2_T res init2_T(H, W, 0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            res[i][j] = mat2D_1[i/H2][j/W2]*mat2D_2[i%H2][j%W2];
        }
    }
    return res;
}

//mat2D_1[i][j]+mat2D_2 for any i, j
template<class T, class T1>
Vec2_T TensorAddition2D(const Vec2_T& mat2D_1, const vector<vector<T1>>& mat2D_2){
    int H1 (Dim1(mat2D_1)), W1 (Dim2(mat2D_1));
    int H2 (Dim1(mat2D_2)), W2 (Dim2(mat2D_2));
    int H (H1*H2), W(W1*W2);
    Vec2_T res init2_T(H, W, 0);
    for(int i(0); i < H; ++i){
        for(int j(0); j < W; ++j){
            res[i][j] = mat2D_1[i/H2][j/W2]+mat2D_2[i%H2][j%W2];
        }
    }
    return res;
}

//dot product
template<class T, class T1>
Vec2_T DotProduct2D(const Vec2_T& mat2D_1, const vector<vector<T1>>& mat2D_2){
    int H1 (Dim1(mat2D_1)), W1 (Dim2(mat2D_1));
    int H2 (Dim1(mat2D_2)), W2 (Dim2(mat2D_2));
    if(W1 != H2){
        cout<<"DotProduct2D: Dim not match"<<endl;
        exit(1);
    }
    Vec2_T res init2_T(H1, W2, 0);
    for(int i(0); i < H1; ++i){
        for(int j (0); j < W2;++j){
            double sum (0);
            for(int p(0); p < W1; ++p){
                sum += mat2D_1[i][p]*mat2D_2[p][j];
            }
            res[i][j] = sum;
        }
    }
    return res;
}

//transpose
template<class T>
Vec2_T Transpose2D(const Vec2_T& mat2D){
    int H (Dim1(mat2D)), W (Dim2(mat2D));
    Vec2_T res init2_T(W, H, 0);
    for(int i(0); i < H; ++i){
        for(int j (0); j < W;++j){
            res[j][i] = mat2D[i][j];
        }
    }
    return res;
}

template<class T>
Vec2_T Reshape2D(const Vec2_T& mat, C_i nH){
    int H (Dim1(mat)), W (Dim2(mat));
    int nW = H*W/nH;
    Vec2_T res init2_T(nH, nW, 0);
    int ii (0), jj (0);
    for(int i(0); i < nH; ++i){
        for(int j(0); j < nW; ++j){
            res[i][j] = mat[ii][jj];
            ++jj;
            if(jj == W){
                ++ii;
                jj = 0;
            }
        }
    }
    return res;
}

#endif /* Matrix_hpp */
