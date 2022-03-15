#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<time.h>
#include<string.h>

char* int2char(int pid)
{
    static char p[5];
    for(int j=4;pid>4;pid/=10){
        p[j--] = pid%10+48;
    }
    return p;
}

int main()
{
    char path[12] = "./tmp_fifo";
    int fd;
    if((fd = open(path,O_WRONLY)) == -1) {
        perror("open fifo err");
        return 1;
    }
    char buf[1024] = { 0 };
    time_t t;
    time(&t);
    char *s = ctime(&t);
    int i = -1;
    while(s[++i] != '\0') {
        buf[i] = s[i];
    }
    pid_t pid = getpid();
    char ownpath[12] = "./";
    char *p = int2char(pid);
    int cnt = 2;
    for(int j = 0; j < 5;j++) {
        if(p[j] != '\0') {
            buf[i++] = p[j];
            ownpath[cnt++] = p[j];
        }
    }
    ownpath[cnt++] = 'r';
    ownpath[cnt++] = 'e';
    ownpath[cnt++] = 'c';
    buf[i++] = '\n';
    for(int j = 0; j < i; j++) {
        printf("%c",buf[j]);
    }
    printf("\n");

    int ret;
    if((ret = write(fd,buf,1024)) == -1) {
        perror("write to server error");
        return 1;
    }
    close(fd);

    memset(buf,'\0',1024);
    if(mkfifo(ownpath,0766) == -1) {
        perror("mkfifo err");
        return 1;
    }
    if((fd = open(ownpath,O_RDONLY)) == -1) {
        perror("open ownfifo err");
        return 1;
    }
    int off = 0;
    while(1) {
        if((off = read(fd,buf,1024)) == -1) {
            perror("read fifo err");
        } else if(off > 0) {
            for(int i = 0; buf[i] != 0; i++) {
                printf("%c",buf[i]);
            }
            printf("\n");
            unlink(ownpath);
            break;
        } else {
            printf("wait");
            sleep(5);
        }
    }
    return 0;
}