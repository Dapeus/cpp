#include<iostream>

#include"user.h"
#include"filesystem.h"
using namespace std;

int main()
{
    FileSystem filesys = FileSystem();
    // filesys.writeFile();
    filesys.readFile();

    system("pause");
    return 0;
}