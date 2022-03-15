//打印自身的函数

#include<stdio.h>

char *recurse="#include<stdio.h> %cchar*recurse=%c%s%c;%cint main(){printf(rescurse,10,34,recurse,34,10,10);}%c";

int main()
{
    printf(recurse,10,34,recurse,34,10,10);
}

