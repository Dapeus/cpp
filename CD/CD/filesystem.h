#pragma once
#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include<iostream>
#include<fstream>
#include<vector>
#include"inode.h"
#include"user.h"

using namespace std;

class FileSystem
{
public:
    vector<inode> inodes; //inodes��
    vector<User> users; //�û���
    vector<int> fi; //����inode
    vector<int> free; //���п�
    string user=""; //�����ն���ʾ�û�
    string path=""; //�����ն���ʾ·��
    int curFile = -1; //���õ�ǰ���ļ���inode��id��
    int curInode = 0; //���õ�ǰ��ʹ�õ�inode��id��

    FileSystem() {};
    ~FileSystem() {};

    void welcome();
    void help();
    void readFile();
    void writeFile();
    void show();
    void init();
    void login();
    void regis();
    void mkdir(string dir);
    void create(string file);
    void dir();
    void cd(string dir);
    void rmdir(string dir);
    void del(string file);
    void find(string file);
    void move(string file, string dir);
    void copy(string file, string dir);
    void open(string file);
    void read();
    void write();
    void close();
    void import(string dir, string file);
    void exp(string file, string dir);
    void head(string file, int num);
    void tail(string file, int num);
};

#endif