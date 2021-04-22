//
//  CABAC.hpp
//  EE669
//
//  Created by Alex on 2019/9/21.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef CABAC_hpp
#define CABAC_hpp

#include "HW2_utli.hpp"

vector<vector<unsigned char>> CABAC_separate(const char* name, int block){
    ifstream ff(name, ios_base::binary);
    vector<unsigned char> v((istreambuf_iterator<char>(ff)), istreambuf_iterator<char>());
    int H = 256;
    int n = H/block;
    vector<vector<unsigned char>> res;
    vector<unsigned char> tmp(H,0);
    int j = 0;
    for(int i = 0; i < v.size(); ++i){
        if(j == H){
            j = 0;
            res.push_back(tmp);
        }
        tmp[j] = v[i];
        ++j;
    }
    res.push_back(tmp);
    vector<vector<unsigned char>> ress(n*n,vector<unsigned char>(block*block,0));
    vector<int> tmp1(n*n,0);
    for(int i=0;i<H;++i){
        for(int j=0;j<H;++j){
            int ii = i/block;
            int jj = j/block;
            ii = n*ii+jj;
            ress[ii][tmp1[ii]] = res[i][j];
            tmp1[ii]++;
        }
    }
    return ress;
}


void CABAC_run(const char* name, int contextbits, const string mapping){
    vector<int> contextlist(contextbits,0);
    FILE *f;
    char *output=new char[64];
    strcat(output, name);
    strcat(output, "_CABAC_encode.dat");
    f = fopen(output, "wb");
    QM coder(f);
    coder.StartQM("encode");
    
    if(mapping == "BP"){
        vector<vector<char>> bits = BitPlane(name);
        for(int k = 0; k < 8; ++k){
            for(int j = 0; j < bits.size(); ++j){
                int newbit = bits[j][k]-'0';
                int context = 0;
                for(int i = 0; i< contextbits; i++){
                    context += pow(2,i)*contextlist[i];
                }
                coder.encode(newbit, context);
                for(int i = contextbits-1; i >= 0; i--){
                    if(i == 0){
                        contextlist[i] = newbit;
                    }
                    else{
                        contextlist[i] = contextlist[i-1];
                    }
                }
            }
        }
        coder.Flush();
        return;
    }
    
    ifstream ff(name, ios_base::binary);
    vector<unsigned char> v((istreambuf_iterator<char>(ff)), istreambuf_iterator<char>());
    map<unsigned char, string> mp = myMapping(name, mapping);
    for(int k = 0; k < v.size(); ++k){
        string bits;
        if(mapping == "ASCII"){
            bits = Byte2Bit(v[k]);
        }
        else if(mapping == "SF" || mapping == "H"){
            bits = mp[v[k]];
        }
        for(int j = 0; j < bits.length(); ++j){
            int newbit = bits[j]-'0';
            int context = 0;
            for(int i = 0; i< contextbits; i++){
                context += pow(2,i)*contextlist[i];
            }
            coder.encode(newbit, context);
            for(int i = contextbits-1; i >= 0; i--){
                if(i == 0){
                    contextlist[i] = newbit;
                }
                else{
                    contextlist[i] = contextlist[i-1];
                }
            }
        }
    }
    coder.Flush();
}

void CABAC_A_run(const char* name, int contextbits, int block){
    vector<int> contextlist(contextbits,0);
    FILE *f;
    char *output=new char[64];
    strcat(output, name);
    strcat(output, "_CABAC_encode.dat");
    f = fopen(output, "wb");
    QM coder(f);
    coder.StartQM("encode");

    vector<vector<unsigned char>> tmp = CABAC_separate(name, block);
    for(int ii=0; ii<tmp.size();++ii){
        vector<vector<char>> bits = BitPlane(tmp[ii]);
        for(int k = 0; k < 8; ++k){
            for(int j = 0; j < bits.size(); ++j){
                int newbit = bits[j][k]-'0';
                int context = 0;
                for(int i = 0; i< contextbits; i++){
                context += pow(2,i)*contextlist[i];
                }
                coder.encode(newbit, context);
                for(int i = contextbits-1; i >= 0; i--){
                    if(i == 0){
                        contextlist[i] = newbit;
                    }
                    else{
                        contextlist[i] = contextlist[i-1];
                    }
                }
            }
        }
    }
    coder.Flush();
}

#endif /* CABAC_hpp */
