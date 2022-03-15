#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void func(int signo, siginfo_t *info, void *p);

int main()
{
    struct sigaction act,oact;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = func;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGINT,&act,&oact);

    while(1) {
        printf("My pid=%d\n",getpid());
        pause();
    }
    return 0;
}

void func(int signo, siginfo_t *info, void *p)
{
    printf("signo=%d\n",signo);
    printf("sender signal pid=%d\n",info->si_pid);
}
