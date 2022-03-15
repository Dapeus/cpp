#include<time.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/resource.h>
#include<arpa/inet.h>

#define BUF_SIZE 1024

void ERROR_EXIT(char *str)
{
    perror(str);
    exit(0);
}

typedef struct {
    int id;
    int fd;
    struct sockaddr_in addr;
} SOCK_UNIT;

void* func(void *argvs)
{
    SOCK_UNIT su = *(SOCK_UNIT*)argvs;
    printf("%d\n",su.fd);
    char *ip_client = malloc(1024*sizeof(char));
    inet_ntop(AF_INET,&su.addr.sin_addr.s_addr,ip_client,1024);
    char buf[BUF_SIZE] = { 0 };
    read(su.fd,buf,BUF_SIZE);
    while(buf[0] != 'q') {
        // fprintf(stdout,"th:%d,client ip:%s,client port:%d,buf:%s\n",su.id,ip_client,ntohs(su.addr.sin_port),buf);
        if(strcmp(buf,"date\n") == 0) {
            memset(buf,'\0',BUF_SIZE);
            time_t timep;
            struct tm *p;
            time(&timep);
            p = gmtime(&timep);
            sprintf(buf,"date:%d-%02d-%02d",1900+p->tm_year,1+p->tm_mon,p->tm_mday);
            write(su.fd,buf,BUF_SIZE);
        } else if(strcmp(buf,"time\n") == 0) { 
            memset(buf,'\0',BUF_SIZE);
            time_t timep;
            struct tm *p;
            time(&timep);
            p = gmtime(&timep);
            sprintf(buf,"time:%02d:%02d:%02d",8+p->tm_hour,p->tm_min,p->tm_sec);
            write(su.fd,buf,BUF_SIZE);
        } else if(strcmp(buf, "index\n") == 0) {
            memset(buf,'\0',BUF_SIZE);
            sprintf(buf,"index");
            write(su.fd,buf,BUF_SIZE);
        } else {
            memset(buf,'\0',BUF_SIZE);
            sprintf(buf,"error command!");
            write(su.fd,buf,BUF_SIZE);
        }
        read(su.fd,buf,BUF_SIZE);
    }
    close(su.fd);
    // fprintf(stdout,"close th:%d,client ip:%s,client port:%d\n",su.id,ip_client,ntohs(su.addr.sin_port));
    return NULL;
}

int main()
{
    // 守护进程
    int pid;
    if((pid = fork()) == -1) {
        ERROR_EXIT("fork err");
    } else if(pid > 0) {
        exit(0);
    }
    if(setsid() == -1) {
        ERROR_EXIT("setsid err");
    }
    if((pid = fork()) == -1) {
        ERROR_EXIT("fork err");
    } else if(pid > 0) {
        exit(0);
    }
    struct rlimit r1;
    if(getrlimit(RLIMIT_NOFILE, &r1) == -1) {
        ERROR_EXIT("getrlimit err");
    }
    int nfiles = r1.rlim_cur = r1.rlim_max;
    for(int i = 3; i < nfiles; i++) {
        close(i);
    }
    int fd;
    if((fd = open("/dev/null",O_RDWR)) == -1) {
        ERROR_EXIT("open err");
    }
    for(int i = 0; i < 3; i++) {
        dup2(fd, i);
    }
    if(fd > 2) {
        close(fd);
    }
    chdir("/");
    umask(0);
    //
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("failed to create socket");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1) {
        perror("failed to bind");
    }
    listen(fd,32); //连接数目最大为5

    // fprintf(stdout,"Server start:\n");
    int id;
    for(id = 0;;id++) {
        struct sockaddr_in n_addr;
        socklen_t addr_len;
        int n_fd;
        if((n_fd = accept(fd, (struct sockaddr *)&n_addr, &addr_len))== -1) {
            perror("failed to accept");
        }
        SOCK_UNIT su;
        su.id = id;
        su.fd = n_fd;
        pthread_t th;
        pthread_create(&th, NULL, func, &su);
        pthread_detach(th);
    }
    
    close(fd);
    return 0;
}

