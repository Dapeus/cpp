#include<iostream>
#include"user.h"
#include"filesystem.h"
using namespace std;

int main()
{
    FileSystem filesys = FileSystem();
    //filesys.init();
    filesys.welcome();
    filesys.readFile();
    filesys.show();
    return 0;
}