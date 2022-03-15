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
    vector<inode> inodes; //inodes表
    vector<User> users; //用户表
    vector<int> fi; //空闲inode
    vector<int> free; //空闲块
    string user=""; //用于终端显示用户
    string path=""; //用于终端显示路径
    int curFile = -1; //放置当前打开文件的inode的id号
    int curInode = 0; //放置当前所使用的inode的id号

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