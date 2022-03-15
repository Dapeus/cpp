#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
    char *cmd = "ls";
    char *argv[3];
    argv[0] = "ls";
    argv[1] = "-alh";
    argv[2] = "";

    printf("%d\n",execvp(cmd,argv));
    return 0;
}