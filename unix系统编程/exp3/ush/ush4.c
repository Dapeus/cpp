#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<limits.h>
#include<setjmp.h>

#define PROMPT_STRING "ush4>>"
#define MAX_BUFFER 256
#define QUIT_STRING "q"
#define BLANK_STRING " "

static sigjmp_buf jumptoprompt;
static volatile sig_atomic_t okaytojump = 0;

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
    // fprintf(stderr,"%s\n",snew);
    if((t = malloc(strlen(snew)+1)) == NULL)
        return -1;
    strcpy(t,snew);
    numtokens = 0;
    if(strtok(t, delimiters) != NULL)
        for(numtokens = 1; strtok(NULL,delimiters) != NULL; numtokens++);
    
    // fprintf(stderr,"%d\n",numtokens);
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
        // fprintf(stderr,"%s\n",**argvp);
        for(i = 1; i < numtokens; i++) {
            *((*argvp)+i) = strtok(NULL,delimiters);
            // fprintf(stderr,"%s\n",*((*argvp)+i));
        }
        return numtokens;
    }
}

int signalsetup(struct sigaction *def, sigset_t *mask, void(*handler)(int))
{
    struct sigaction catch;

    catch.sa_handler = handler;
    def->sa_handler = SIG_DFL;
    catch.sa_flags = 0;
    def->sa_flags = 0;
    if((sigemptyset(&def->sa_mask)) == -1 || 
       (sigemptyset(&(catch.sa_mask)) == -1) ||
       (sigaddset(&(catch.sa_mask), SIGINT) == -1) ||
       (sigaddset(&(catch.sa_mask), SIGQUIT) == -1) ||
       (sigaction(SIGINT, &catch, NULL) == -1) ||
       (sigaction(SIGQUIT, &catch, NULL) == -1) ||
       (sigemptyset(mask) == -1) ||
       (sigaddset(mask, SIGINT) == -1) ||
       (sigaddset(mask, SIGQUIT) == -1))
       return -1;
    return 0;
}

void executecmd(char *incmd)
{
    char **chargv;
    if(makeargv(incmd, BLANK_STRING, &chargv) <= 0) {
        fprintf(stderr,"Failed to parse command link\n");
        exit(1);
    }
    execvp(chargv[0],chargv);
    perror("Failed to execute command");
    exit(1);
}

static void jumphd(int signalnum)
{
    if(!okaytojump)
        return;
    okaytojump = 0;
    siglongjmp(jumptoprompt,1);
}

int main(void)
{
    sigset_t blockmask;
    pid_t childpid;
    struct sigaction defaction;
    char inbuf[MAX_CANON];
    int len;

    if(signalsetup(&defaction, &blockmask,jumphd) == -1) {
        perror("Failed to set up shell signal handing");
        return 1;
    }
    if(sigprocmask(SIG_BLOCK, &blockmask, NULL) == -1) {
        perror("Failed to block signals");
        return 1;
    }
    while(1) {
        if(fputs(PROMPT_STRING, stdout) == EOF)
            continue;
        if(fgets(inbuf, MAX_CANON, stdin) == NULL)
            continue;
        len = strlen(inbuf);
        // for(int i = 0; i < len; i++) {
        //     fprintf(stderr,"%d ",inbuf[i]);
        // }
        if(inbuf[len-1] == '\n')
            inbuf[len-1] = 0;
        if(strcmp(inbuf, QUIT_STRING) == 0) // quit main
            break;
        if((childpid = fork()) == -1) {
            perror("Failed to fork child to execute command");
        } else if(childpid == 0) {
            if((sigaction(SIGINT, &defaction, NULL) == -1) || (sigaction(SIGQUIT, &defaction, NULL) == -1) || (sigprocmask(SIG_UNBLOCK, &blockmask, NULL)) == -1) {
                perror("Failed to set signal handling for command");
                return 1;
            }
            executecmd(inbuf);
            return 1;
        }
        wait(NULL);
    }
    return 0;
}
