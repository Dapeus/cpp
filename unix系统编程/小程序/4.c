//获得uid,gid,euid,egid

#include<stdio.h>
#include<unistd.h>

int main()
{
    printf("gid::%d\n",getuid());
    printf("uid::%d\n",getgid());
    printf("euid::%d\n",geteuid());
    printf("egid::%d\n",getegid());

    return 0;
}