#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>

#define MAX_BUFFER 256
#define QUIT_STRING "q"

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

int main(void)
{
    char **chargv;
    char inbuf[MAX_BUFFER];

    while (1)
    {
        gets(inbuf);
        if(strcmp(inbuf,QUIT_STRING) == 0)
            return 0;
        if((fork() == 0) && (makeargv(inbuf," ",&chargv) > 0))
            execvp(chargv[0],chargv);
        wait(NULL);
    }
}