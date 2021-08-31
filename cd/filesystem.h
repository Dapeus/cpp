#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include<vector>
#include<iostream>
#include<fstream>
#include"inode.h"
#include"user.h"

using namespace std;

class FileSystem
{
private:
    vector<inode> inodes;
    int curId;
    vector<User> users;
public:
    FileSystem(){};
    ~FileSystem(){};

    void readFile();
    void writeFile();
};
void FileSystem::writeFile(){
    ofstream fs("D:\\code\\cpp\\cd\\sys.dat",ios::out | ios::binary);
    User u;
    cin>>u.acc>>u.pass;
    cout<<sizeof(u)<<endl;
    fs.write((char*)&u,sizeof(u));
    fs.close();
}

void FileSystem::readFile(){
    User u;
    ifstream fs("D:\\code\\cpp\\cd\\sys.dat",ios::binary|ios::in);
    fs.read((char*)&u,sizeof(u));
    cout<<u.acc<<" "<<u.pass<<endl;
    fs.close();
}
#endif