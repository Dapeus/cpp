#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#define MAX_LINE 1024   /* max line size */
#define MAX_ARGS 128   /* max args on a command line */
#define MAX_JOBS 16   /* max jobs at any point in time */
#define MAX_JID 1<<16   /* max job ID */
#define MAX_BUF 100

void handler1(int sig)
{
    pid_t pid; 
    if ((pid = waitpid(-1, NULL, 0)) < 0) 
        perror("waitpid error"); 
    fprintf(stdout,"Handler reaped child %d\n", (int)pid); 
    sleep(2);
    return;
}

int main()
{
    int i, n; 
    char buf[MAX_BUF]; 
    if (signal(SIGCHLD, handler1) == SIG_ERR)
        perror("signal error"); /* Parent creates children */ 
    for (i = 0; i < 3; i++) {
        if (fork() == 0) {
            printf("Hello from child %d\n", (int)getpid());
            sleep(1);
            exit(0);
        }
    } 
    /* Parent waits for terminal input and then processes it */ 
    if((n = read(STDIN_FILENO, buf, sizeof(buf))) < 0)
        printf("read"); 
    printf("Parent processing input\n"); 
    while(1);

    return 0;
}