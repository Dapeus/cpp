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
    char path[12] = "temp_fifo";
    mode_t mode = 0766;
    if(mkfifo(path,mode) == -1) {
        perror("mkfifo err");
        return 1;
    }
    time_t t;
    int i =  0, fd;
    char buf[1024];
    if((fd = open(path,O_RDONLY)) == -1) {
        perror("open ownfifo err");
        return 1;
    }
    int off;
    while(1) {
        if((off = read(fd,buf,1024)) == -1) {
            perror("read file error");
            return 1;
        } 
        if(off > 0) {
            char pid[8];
            int j = 0;
            for(int i = 25; buf[i] != '\0'; i++) {
                pid[j++] = buf[i];
            }
            pid[j-1] = 'r';
            pid[j++] = 'r';
            pid[j] = 'r';
            time(&t);
            char *s = ctime(&t);
            char inb[1024] = { 0 };
            int k = -1;
            while(s[++k] != '\0') {
                inb[k] = s[k];
            }
            inb[k++] = '\n';
            for(int i = 0; i < j-2; i++) {
                inb[k++] = pid[i];
            }
            inb[k++] = '\n';
            int own_pid = getpid();
            char *p = int2char(own_pid);
            for(int i = 0; i < 5; i++) {
                if(p[i] != '\0') {
                    inb[k++] = p[i];
                }
            }
            printf("\n");
            for(int i = 0; inb[i] != '\0'; i++) {
                printf("%c",inb[i]);
            }

            int fd_c;
            printf("\n");
            for(int i = 0; i < 8; i++) {
                printf("%c",pid[i]);
            }
            if((fd_c = open(pid,O_WRONLY|O_NONBLOCK)) == -1) {
                perror("open client file err");
            } else {
                write(fd_c,inb,1024);
            }
            printf("\n");
        } else {
            printf("off:%d\n",off);
            sleep(5);
        }
    }
    unlink(path);
    return 0;
}