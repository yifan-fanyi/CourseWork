//
//  BinaryTree.hpp
//  EE569
//
//  Created by Alex on 2019/2/17.
//  Copyright © 2019 Alex. All rights reserved.
//

#ifndef BinaryTree_hpp
#define BinaryTree_hpp

#include "core.hpp"

//Binary Dictionary using tree
//accept 0 1 and d:(no decide)
//only can search fixed length
//label: decided by position, else use '_' replace
//multipy labels in a node would be combined
typedef struct BinaryDict{
    string label;
    BinaryDict *left;
    BinaryDict *right;
    BinaryDict(string str) : label(str), left(NULL), right(NULL) {}
}BinaryDict;

//build a binary tree branch when meet d
void BuildBinaryBrach(BinaryDict* tmp, C_s& str, C_s& label, int j);

//build a binary tree
BinaryDict* BuildBinaryDict(C_Vec_s& s, C_Vec_s& label);

//print check the tree
void PrintBinaryDict(BinaryDict* head);

//combine labels in a node
string CombineLabel(C_s& s1, C_s& s2);

bool SearchBinaryDict(BinaryDict* head, C_Vec2_ui8& Im2D, C_i& i, C_i& j, C_i& task, C_i& H, C_i& W);

#endif /* BinaryTree_hpp */
