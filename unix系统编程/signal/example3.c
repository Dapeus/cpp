// 捕获SIGINT信号

#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void my_handler(int sig);

int main()
{
    struct sigaction act,oact;

    act.sa_handler = my_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    sigaction(SIGINT,&act,&oact);

    while(1) {
        printf("running,please ctrl+c\n");
        pause();
    }
}

void my_handler(int sig)
{
    printf("I have got SIGINT signal\n");   
}
