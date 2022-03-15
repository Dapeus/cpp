#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

static void perror_exit(char *s)
{
    perror(s);
    exit(1);
}

void executedcmd(char *cmds)
{
    int child;
    int count;
    int fds[2];
    int i;
    char **pipelist;

    count = makeargv(cmds, "|", &pipelist);
    if(count <= 0) {
        fprintf(stderr,"Failed to find any commands\n");
        exit(1);
    }
    for(i = 0; i < count-1; i++) {
        if(pipe(fds) == -1)
            perror_exit("Failed to create pipes");
        else if((child = fork()) == -1)
            perror_exit("Failed to create process to run command");
        else if(child) {
            if(dup2(fds[1], STDOUT_FILENO) == -1)
                perror_exit("Failed to connect pipeline");
            executeredirect(pipelist[i], i==0, 0);
            exit(1);
        }
        if(dup2(fds[0], STDIN_FILENO) == -1)
            perror_exit("Failed to do final close");
        if(close(fds[0] || close(fds[1])))
            perror_exit("Failed to do final close");
    }
    executeredirect(pipelist[i], i==0, 1);
    exit(1);
}