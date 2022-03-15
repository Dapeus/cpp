#pragma once
#ifndef INODE_H
#define INODE_H
#include<iostream>
using namespace std;
class inode
{
public:
    char owner[50] = { 0 }; //创建者账户
    int dir; //是否是目录 1：是，0：否
    int id; //inode的id，用于标识
    int prev; //其上级inode的id，用于标识
    char filename[50] = { 0 }; //文件名或目录名
    int block[100]; //所占有的块id号
    inode() {
        id = 0;
        prev = 0;
        for (int i = 0; i < 100; i++) {
            block[i] = -1;
        }
    };
};

#endif