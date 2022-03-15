#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<limits.h>
#include<setjmp.h>

#define PROMPT_STRING "ush8>>"
#define MAX_BUFFER 256
#define QUIT_STRING "q"
#define BACK_SYMBOL '&'
#define BLANK_STRING " "
#define FFLAG (O_WRONLY | O_CREAT | O_TRUNC)
#define FMODE (S_IRUSR | S_IWUSR)

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
    return 0;
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

// void executecmd(char *incmd)
// {
//     char **chargv;
//     if(makeargv(incmd, BLANK_STRING, &chargv) <= 0) {
//         fprintf(stderr,"Failed to parse command link\n");
//         exit(1);
//     }
//     if(strcmp("ps",chargv[0]) == 0) {
//         char s[1000] = { 0 };
//         sprintf(s," --ppid %d",getppid());
//         char in[1000] = { 0 };
//         int j = 0;
//         for(int i = 1; i < (int)strlen(incmd); i++) {
//             if(incmd[i] == 's' && incmd[i-1] == 'p') {
//                 j = i+2;
//                 break;
//             }
//         }
//         for(int i = 0; i < j; i++) {
//             in[i] = incmd[i];
//         }
//         strcat(in,s);
//         // fprintf(stderr,"%s\n",in);
//         for(int i = j+strlen(s),k = 2; i < strlen(incmd)+strlen(s)+1; i++) {
//             fprintf(stderr,"%d\n",i);
//             in[i] = incmd[k++];
//         }
//         fprintf(stderr,"%s\n",in);
//         if(*(in + strlen(in)-1) == '\n')
//             *(in + strlen(in)-1) = 0;
//         if(makeargv(in, BLANK_STRING, &chargv) <= 0) {
//             fprintf(stderr,"Failed to parse command link\n");
//             exit(1);
//         }
//     }
//     execvp(chargv[0],chargv);
//     perror("Failed to execute command");
//     exit(1);
// }

static void jumphd(int signalnum)
{
    if(!okaytojump)
        return;
    okaytojump = 0;
    siglongjmp(jumptoprompt,1);
}

int parseandredirectin(char *cmd)
{
    int error;
    int infd;
    char *infile;
    if((infile = strchr(cmd, '<')) == NULL)
        return 0;
    *infile = 0;
    infile = strtok(infile+1," \t");
    if(infile == NULL)
        return 0;
    if((infd = open(infile, O_RDONLY)) == -1)
        return -1;
    if(dup2(infd,STDIN_FILENO) == -1) {
        error = errno;
        close(infd);
        errno = error;
        return -1;
    }
    return close(infd);
}

int parseandredirectout(char *cmd)
{
    int error;
    int outfd;
    char *outfile;
    if((outfile = strchr(cmd, '>')) == NULL)
        return 0;
    *outfile = 0;
    outfile = strtok(outfile+1," \t");
    if(outfile == NULL)
        return 0;
    if((outfd = open(outfile, FFLAG, FMODE)) == -1)
        return -1;
    if(dup2(outfd,STDOUT_FILENO) == -1) {
        error = errno;
        close(outfd);
        errno = error;
        return -1;
    }
    return close(outfd);
}

void executeredirect(char *s, int in, int out)
{
    char **chargv;
    char *pin;
    char *pout;

    if(in && ((pin = strchr(s,'<')) != NULL) && out && ((pout = strchr(s,'>')) != NULL) && (pin > pout)) {
        if(parseandredirectin(s) == -1) {
            perror("Failed to redirect input");
            return;
        }
        in = 0;
    }
    if(out && parseandredirectout(s) == -1)
        perror("Failed to redirect output");
    else if(in && parseandredirectin(s) == -1)
        perror("Failed to redirect input");
    else if(makeargv(s," \t",&chargv) <= 0)
        fprintf(stderr,"Failed to parse command line aaaa\n");
    else {
        execvp(chargv[0],chargv);
        perror("Failed to execute command");
    }
    exit(1);
}

static void perror_exit(char *s)
{
    perror(s);
    exit(1);
}

void executecmd(char *cmds)
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

int main(void)
{
    char *backp;
    int inbackground;
    sigset_t blockmask;
    pid_t childpid;
    struct sigaction defhandler;
    char inbuf[MAX_CANON];

    if(signalsetup(&defhandler, &blockmask,jumphd) == -1) {
        perror("Failed to set up shell signal handing");
        return 1;
    }

    while(1) {
        if((sigsetjmp(jumptoprompt,1)) && (fputs("\n",stdout) == EOF))
            continue;
        okaytojump = 1;
        if(fputs(PROMPT_STRING, stdout) == EOF)
            continue;
        if(fgets(inbuf, MAX_CANON, stdin) == NULL)
            continue;
        // for(int i = 0; i < len; i++) {
        //     fprintf(stderr,"%d ",inbuf[i]);
        // }
        if(*(inbuf + strlen(inbuf)-1) == '\n')
            *(inbuf + strlen(inbuf)-1) = 0;
        if(strcmp(inbuf, QUIT_STRING) == 0) // quit main
            break;
        if((backp = strchr(inbuf,BACK_SYMBOL)) == NULL)
            inbackground = 0;
        else {
            inbackground = 1;
            *backp = 0;
        }
        char **chargv;
        if(makeargv(inbuf, BLANK_STRING, &chargv) <= 0) {
            fprintf(stderr,"Failed to parse command link bb\n");
            exit(1);
        }
        if(strcmp(chargv[0],"cd") == 0) {
            chdir(chargv[1]);
            continue;
        }
        if(sigprocmask(SIG_BLOCK, &blockmask, NULL) == -1)
            perror("Failed to block signals");
        if((childpid = fork()) == -1) {
            perror("Failed to fork child to execute command");
        } else if(childpid == 0) {
            if(inbackground && (setpgid(0,0)) == -1)
                return 1;
            if((sigaction(SIGINT, &defhandler, NULL) == -1) || (sigaction(SIGQUIT, &defhandler, NULL) == -1) || (sigprocmask(SIG_UNBLOCK, &blockmask, NULL)) == -1) {
                perror("Failed to set signal handling for command");
                return 1;
            }
            // 打印子进程的id，父进程id，进程组id
            // printf("%d\n",getpid());
            // printf("%d\n",getppid());
            // printf("%d\n",getpgid(0));
            executecmd(inbuf);
            perror("Failed to set siganl handing for command");
            return 1;
        }
        if(sigprocmask(SIG_UNBLOCK, &blockmask, NULL) == -1)
            perror("Failed to to unblock signals");
        if(!inbackground) {
            waitpid(childpid, NULL, 0);
        }
        while(waitpid(-1, NULL, WNOHANG) > 0); //wait background procs
        /*
            waitpid() first argument == -1 --> wait all child procsd
                      third argument == WNOHANG --> don't wait anyway
            return value: 0   -> child procs not over
                          pid -> child procs over
        */
    }
    return 0;
}
