#pragma once
#ifndef INODE_H
#define INODE_H
#include<iostream>
using namespace std;
class inode
{
public:
    char owner[50] = { 0 }; //�������˻�
    int dir; //�Ƿ���Ŀ¼ 1���ǣ�0����
    int id; //inode��id�����ڱ�ʶ
    int prev; //���ϼ�inode��id�����ڱ�ʶ
    char filename[50] = { 0 }; //�ļ�����Ŀ¼��
    int block[100]; //��ռ�еĿ�id��
    inode() {
        id = 0;
        prev = 0;
        for (int i = 0; i < 100; i++) {
            block[i] = -1;
        }
    };
};

#endif