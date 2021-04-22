//
//  HW2_utli.hpp
//  EE669
//
//  Created by Alex on 2019/9/18.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW2_utli_hpp
#define HW2_utli_hpp

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <cmath>
#include <string>
#include <algorithm>
#include <math.h>
#include <cstring>

#include "HW1_utli.hpp"
#include "ShannonFano.hpp"
#include "Huffman_GS.hpp"
#include "QMcoder_src.hpp"

using namespace std;

string Bytes2Bits(const vector<unsigned char>& data){
    string res;
    for(int i = 0; i < data.size(); ++i){
        int t = (int)data[i];
        string tmp = "";
        while(t > 0){
            tmp = ((t%2==0) ? "0" : "1") + tmp;
            t >>= 1;
        }
        while(tmp.length() < 8){
            tmp = "0" + tmp;
        }
        res += tmp;
    }
    return res;
}

string Byte2Bit(const unsigned char& data){
    int t = (int)data;
    string tmp = "";
    while(t > 0){
        tmp = ((t%2==0) ? "0" : "1") + tmp;
        t >>= 1;
    }
    while(tmp.length() < 8){
        tmp = "0" + tmp;
    }
    return tmp;
}

vector<char> Byte2Bit_v(const unsigned char& data){
    vector<char> res;
    string tmp = Byte2Bit(data);
    for(int i = 0; i < 8; ++i){
        res.push_back(tmp[i]);
    }
    return res;
}

string Bit2Byte(string& tmp){
    string res;
    while(tmp.length() > 8){
        int len = (int)tmp.length();
        int x = 0;
        for(int j = 0; j < 8; ++j){
            int t = (tmp[j] == '0') ? 0 : 1;
            x += pow(2,7-j)*t;
        }
        res += (char)x;
        tmp = tmp.substr(8, len-8);
    }
    return res;
}

void toInt16(unsigned int& C){
    while(C >= 65536){
        C -= 65536;
    }
}

vector<vector<char>> BitPlane(const char* name){
    ifstream f(name, ios_base::binary);
    vector<unsigned char> v((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    vector<vector<char>> Bits;
    for(int i = 0; i < v.size(); ++i){
        Bits.push_back(Byte2Bit_v(v[i]));
    }
    return Bits;
}

vector<vector<char>> BitPlane(vector<unsigned char>& v){
    vector<vector<char>> Bits;
    for(int i = 0; i < v.size(); ++i){
        Bits.push_back(Byte2Bit_v(v[i]));
    }
    return Bits;
}

map<unsigned char, string> myMapping(const char* name, const string& opt){
    if(opt == "SF"){
        return SF_getDict(name);
    }
    else if(opt == "H"){
        ifstream f(name, ios_base::binary);
        vector<unsigned char> v((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
        return H_CodingDict(v);
    }
    else{
        map<unsigned char, string> mp;
        return mp;
    }
}

#endif /* HW2_utli_hpp */
