#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define BLANK_STRING " "

void executecmd(char *incmd)
{
    char **chargv;
    if(makeargv(incmd, BLANK_STRING, &chargv) <= 0) {
        fprintf(stderr,"Failed to parse command link");
        exit(1);
    }
    execvp(chargv[0],chargv);
    perror("Failed to execute command");
    exit(1);
}
