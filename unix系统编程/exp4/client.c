#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>

#define BUF_SIZE 1024

int main()
{
    int fd;
    if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1) {
        perror("failed to create socket");
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int test = 0;
    int n_fd;
    if((n_fd = connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr))) == -1) {
        perror("failed to connect");
    } else {
        fprintf(stdout,"client start(press q to quit)\n");
        char buf[BUF_SIZE] = { 0 };
        while(1) {
            write(STDOUT_FILENO,"client>",8);
            memset(buf,'\0',BUF_SIZE);
            fgets(buf,BUF_SIZE,stdin);
            if(buf[0] == 'q') {
                write(fd,buf,BUF_SIZE);
                close(fd);
                close(n_fd);
                break;
            }
            write(fd,buf,BUF_SIZE);
            memset(buf,'\0',BUF_SIZE);
            read(fd,buf,BUF_SIZE);
            fprintf(stdout,"%s\n",buf);
        }
    }
    return 0;
}
