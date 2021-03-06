#include<errno.h>
#include<stdio.h>
#include<unistd.h>

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
        fprintf(stderr,"Failed to parse command line\n");
    else {
        execvp(chargv[0],chargv);
        perror("Failed to execute command");
    }
    exit(1);
}