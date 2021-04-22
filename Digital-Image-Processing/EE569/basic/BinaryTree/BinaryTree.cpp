//
//  BinaryTree.cpp
//  EE569
//
//  Created by Alex on 2019/2/17.
//  Copyright © 2019 Alex. All rights reserved.
//

#include "BinaryTree.hpp"

void BuildBinaryBrach(BinaryDict* tmp, C_s& str, C_s& label, int j){
    for(; j < str.length(); ++j){
        if(str[j] == '0'){
            if(tmp->left == NULL){
                tmp->left = new BinaryDict(label);}
            if(tmp->label != label){
                tmp->label = CombineLabel(tmp->label, label);}
            tmp = tmp->left;}
        else if(str[j] == '1'){
            if(tmp->right == NULL ){
                tmp->right = new BinaryDict(label);}
            if(tmp->label != label){
                tmp->label = CombineLabel(tmp->label, label);}
            tmp = tmp->right;}
        else{//s[i][j] = 'd'
            if(tmp->label != label){
                tmp->label = CombineLabel(tmp->label, label);}
            if(tmp->left == NULL ){
                tmp->left = new BinaryDict(label);}
            if(tmp->right == NULL ){
                tmp->right = new BinaryDict(label);}
            BuildBinaryBrach(tmp->left, str, label, j+1);
            BuildBinaryBrach(tmp->right, str, label, j+1);
            return;
        }
    }
}

BinaryDict* BuildBinaryDict(C_Vec_s& s, C_Vec_s& label){
    BinaryDict* head(new BinaryDict(""));
    int num(Dim1(s));
    int length(Dim2(s));
    for(int i(0); i < num; ++i){
        BinaryDict* tmp = head;
        for(int j(0); j < length; ++j){
            if(s[i][j] == '0'){
                if(tmp->left == NULL){
                    tmp->left = new BinaryDict(label[i]);}
                if(tmp->label != label[i]){
                    tmp->label = CombineLabel(tmp->label, label[i]);}
                tmp = tmp->left;}
            else if(s[i][j] == '1'){
                if(tmp->right == NULL ){
                    tmp->right = new BinaryDict(label[i]);}
                if(tmp->label != label[i]){
                    tmp->label = CombineLabel(tmp->label, label[i]);}
                tmp = tmp->right;}
            else{//s[i][j] = 'd'
                if(tmp->label != label[i]){
                    tmp->label = CombineLabel(tmp->label, label[i]);}
                if(tmp->left == NULL ){
                    tmp->left = new BinaryDict(label[i]);}
                if(tmp->right == NULL ){
                    tmp->right = new BinaryDict(label[i]);}
                BuildBinaryBrach(tmp->left, s[i], label[i], j+1);
                BuildBinaryBrach(tmp->right, s[i], label[i], j+1);
                break;
            }
        }
    }
    return head;
}

void PrintBinaryDict(BinaryDict* head){
    if(head == NULL){return;}
    queue<BinaryDict*> qu;
    qu.push(head);
    while(!qu.empty()){
        int size(Dim1(qu));
        for(int i(0); i < size; ++i){
            if(qu.front() != NULL){
                cout<<qu.front()->label<<"  ";
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

string CombineLabel(C_s& s1, C_s& s2){
    string res;
    for(int i(0); i < Dim1(s1); ++i){
        if(s1[i] != '_'){
            res += s1[i];
        }
        else{
            res += s2[i];
        }
    }
    return res;
}

C_Vec2_i loc({{0,0},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1}});
bool SearchBinaryDict(BinaryDict* head, C_Vec2_ui8& Im2D, C_i& i, C_i& j, C_i& task, C_i& H, C_i& W){
    BinaryDict* tmp = head;
    int ii(0);
    bool t(0);
    while(ii < 9){
        if(tmp == NULL){return false;}
        if(i > 0 && j > 0 && j < W-1 && i < H-1){
            t = Im2D[i+loc[ii][0]][j+loc[ii][1]];
        }
        else{
            int x(i+loc[ii][0]), y(j+loc[ii][1]);
            if(x >= 0 && y >= 0 && y < W && x < H){
                t = Im2D[x][y];
            }
        }
        if(t > 0 && tmp->right != NULL && tmp->right->label[task] != '_'){
            tmp = tmp->right;
        }
        else if(t == 0 && tmp->left != NULL && tmp->left->label[task] != '_'){
            tmp = tmp->left;
        }
        else{return false;}
        ++ii;
    }
    return true;
}
