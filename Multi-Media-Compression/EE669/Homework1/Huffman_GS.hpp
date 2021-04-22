//
//  Huffman_GS.hpp
//  EE669
//
//  Created by Alex on 2019/9/6.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef Huffman_GS_hpp
#define Huffman_GS_hpp

#include "HW1_utli.hpp"

struct Tree{
    Tree* left;
    Tree* right;
    char isleaf;
};

int H_selectMin(vector<double>& freq){
    double min1 = INT_MAX;
    int idx = -1;
    for(int i = 0; i < freq.size(); ++i){
        if(freq[i] > 0.000000001 &&  freq[i] < min1){
            min1 = freq[i];
            idx = i;
        }
    }
    return idx;
}

Tree* H_Buildtree(const vector<double>& freq, const vector<unsigned char>& dict){
    vector<double> tfreq(freq);
    vector<Tree*> tmp(dict.size(), NULL);
    int idx, idx1;
    int t = 0;
    while(1){
        idx = H_selectMin(tfreq);
        Tree* node = new Tree;
        Tree* n1 = new Tree;
        Tree* n2 = new Tree;
        if(tmp[idx] != NULL){
            n1 = tmp[idx];
        }
        else{
            n1->isleaf = dict[idx];
            n1->left = NULL;
            n1->right = NULL;
        }
        node->left = n1;
        tfreq[idx] = tfreq[idx]-1;
        
        idx1 = H_selectMin(tfreq);
        if(idx1 < 0){ t = idx;break;}
        if(tmp[idx1] != NULL){
            n2 = tmp[idx1];
        }
        else{
            n2->isleaf = dict[idx1];
            n2->left = NULL;
            n2->right = NULL;
        }
        node->right = n2;
        node->isleaf = '0';
        //H_PrintBinaryDict(node);
        tfreq[idx1] = tfreq[idx1] - 1;
        tfreq[idx] = tfreq[idx] + tfreq[idx1] + 2;
        tmp[idx] = node;
        tmp[idx1] = NULL;
    }
    return tmp[t];
}

void H_PrintBinaryDict(Tree* head){
    if(head == NULL){return;}
    queue<Tree*> qu;
    qu.push(head);
    while(!qu.empty()){
        int size = (int)qu.size();
        for(int i(0); i < size; ++i){
            if(qu.front() != NULL){
                cout<<qu.front()->isleaf<<"  ";
                qu.push(qu.front()->left);
                qu.push(qu.front()->right);
            }
            else{
                cout<<"..  ";
            }
            qu.pop();
        }
        cout<<endl;
    }
}

void H_ConvertTree(Tree* head, map<unsigned char, string>& mp, string code){
    if(head == NULL){
        return;
    }
    if(head->left == NULL && head->right == NULL){
        mp[head->isleaf] = code;
        return;
    }
    if(head->left != NULL){
        H_ConvertTree(head->left, mp, code+'0');
    }
    if(head->right != NULL){
        H_ConvertTree(head->right, mp, code+'1');
    }
}

map<unsigned char, string> H_CodingDict(const vector<unsigned char>& data){
    vector<double> freq;
    vector<unsigned char> dict;
    GetStatics(data, freq, dict);
    map<unsigned char, string> mp;
    Tree* head = H_Buildtree(freq, dict);
    string s;
    //H_PrintBinaryDict(head);
    H_ConvertTree(head, mp, s);
    double len = 0;
    for(int i = 0; i < freq.size(); ++i){
        len += freq[i]*((double)mp[dict[i]].length());
    }
    //cout<<"Average code length: "<<len<<endl;

    return mp;
}

void H_encode(const vector<unsigned char>& data, map<unsigned char, string> mp, const string& name){
    ofstream file("./"+name+"_H_encode.dat");
    string tmp;
    for(int i = 0; i < data.size(); ++i){
        tmp = tmp+mp[data[i]];
        while(tmp.length() > 8){
            int len = (int)tmp.length();
            int x = 0;
            for(int j = 0; j < 8; ++j){
                int t = (tmp[j] == '0') ? 0 : 1;
                x += pow(2,7-j)*t;
            }
            
            file<<(char)x;
            tmp = tmp.substr(8, len);
        }
    }
    int x = 0;
    for(int j = 0; j < tmp.length(); ++j){
        int t = tmp[j] == 0 ? 0 : 1;
        x += pow(2,7-j)*t;
    }
    file<<(char)x;
}

void H_run(const string& name){
    ifstream f(name, ios_base::binary);
    vector<unsigned char> v((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    if(v.size() <= 0){
        cout<<"Can not read File."<<endl;
        return;
    } 
    map<unsigned char, string> mp = H_CodingDict(v);
    map<unsigned char, string>::iterator it = mp.begin();
    cout<<"Huffman encoding dict:"<<endl;
    cout<<"   "<<"char"<<"  "<<"code"<<endl;
    int c = 1;
    while(it != mp.end()){
        cout<<"    "<<(int)it->first<<" &  "<<it->second;
        if(c==5){
            c=0;
            cout<<"\\\\"<<endl;
        }
        else{
            cout<<"&";
        }
        c+=1;
        it++;
    }
    cout<<endl;
    H_encode(v, mp, name);
}

#endif /* Huffman_GS_hpp */
