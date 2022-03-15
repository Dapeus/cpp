#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<limits.h>

#define PROMPT_STRING "ush2>>"
#define MAX_BUFFER 256
#define QUIT_STRING "q"
#define BLANK_STRING " "

int makeargv(const char *s, const char *delimiters, char ***argvp)
{
    int error;
    int i;
    int numtokens;
    const char *snew;
    char *t;

    if((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
        errno = EINVAL;
        return -1;
    }

    *argvp = NULL;
    snew = s + strspn(s,delimiters);
    if((t = malloc(strlen(snew)+1)) == NULL)
        return -1;
    strcpy(t,snew);
    numtokens = 0;
    if(strtok(t, delimiters) != NULL)
        for(numtokens = 1; strtok(NULL,delimiters) != NULL; numtokens++);
    
    if((*argvp = malloc((numtokens+1)*sizeof(char *))) == NULL) {
        error = errno;
        free(t);
        errno = error;
        return -1;
    }
    if(numtokens == 0)
        free(t);
    else {
        strcpy(t,snew);
        **argvp = strtok(t, delimiters);
        for(i = 1; i < numtokens; i++)
            *((*argvp)+i) = strtok(NULL,delimiters);
        return numtokens;
    }
}

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

int main(void)
{
    pid_t childpid;
    char inbuf[MAX_CANON];
    int len;

    while (1)
    {
        if(fputs(PROMPT_STRING,stdout) == EOF)
            continue;
        if(fgets(inbuf, MAX_CANON, stdin) == NULL)
            continue;
        len = strlen(inbuf);
        if(inbuf[len-1] == '\n')
            inbuf[len-1] = 0;
        if(strcmp(inbuf,QUIT_STRING) == 0)
            break;
        if((childpid = fork()) == -1)
            perror("Failed to fork child");
        else if(childpid == 0) {
            executecmd(inbuf);
            return 1;
        } else {
            wait(NULL);
        }
    }
    return 0;
}
