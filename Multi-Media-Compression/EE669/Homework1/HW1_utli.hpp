//
//  HW1_utli.hpp
//  EE669
//
//  Created by Alex on 2019/9/6.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef HW1_utli_hpp
#define HW1_utli_hpp

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

using namespace std;

void mySort(vector<double>& freq, vector<unsigned char>& label){
    for (int i = 0; i < freq.size(); i++){
        for (int j = 0; j < freq.size()-i-1; j++){
            if (freq[j] > freq[j + 1]){
                double temp;
                temp = freq[j + 1];
                freq[j+1] = freq[j];
                freq[j] = temp;
                char tempc;
                tempc = label[j + 1];
                label[j+1] = label[j];
                label[j] = tempc;
            }
        }
    }
}

void GetStatics(const vector<unsigned char>& data, vector<double>& freq, vector<unsigned char>& dict){
    map<unsigned char, int> mp;
    for(int i = 0; i < data.size(); ++i){
        mp[data[i]]++;
    }
    map<unsigned char, int>::iterator iter = mp.begin();
    while(iter != mp.end()){
        freq.push_back((double)iter->second/(double)data.size());
        dict.push_back(iter->first);
        iter++;
    }
    mySort(freq, dict);
    /*
    cout<<"Statics of the data: "<<endl;
    cout<<"   "<<"char"<<"  "<<"freq"<<endl;
    for(int i = 0; i < freq.size(); ++i){
        cout<<"    "<<(int)dict[i]<<"    "<<freq[i]<<endl;
    }
     */
}

long int FileSize(const char* filename){
    FILE *fp=fopen(filename,"r");
    if(!fp) return -1;
    fseek(fp,0L,SEEK_END);
    long int size = ftell(fp);
    fclose(fp);
    return size;
}

double AvgCodeLength(vector<double>& freq, vector<string>& code){
    double len = 0;
    for(int i = 0; i < freq.size(); ++i){
        len += freq[i]*((double)code[i].length());
    }
    return len;
}

double Entropy(vector<double>& freq){
    double len = 0;
    for(int i = 0; i < freq.size(); ++i){
        len -= log2(freq[i])*freq[i];
    }
    return len;
}
#endif /* HW1_utli_hpp */
