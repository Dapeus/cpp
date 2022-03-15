//打印环境列表

#include<stdio.h>
#include<stdlib.h>

int main(int argc,char **argv,char **environ)
{
    for(int i = 0; environ[i] != NULL; i++){
        printf("%s\n",environ[i]);
    }
    return 0;
}