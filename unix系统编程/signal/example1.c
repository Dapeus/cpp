#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

void printsigset(sigset_t *set)
{
	int jud;
	for(int i = 0; i < 32; i++) {
		if(sigismember(set,i))
			fprintf(stdout,"1");
		else
			fprintf(stdout,"0");
	}
	fprintf(stdout,"\n");
}

int main()
{
	sigset_t s,p;
	sigemptyset(&s);
	sigaddset(&s,SIGINT);

	// signal set s is used as the masked signal set for the current process
	// try to annotate if(){} then ctrl+c
	if(sigprocmask(SIG_BLOCK,&s,NULL) == -1) {
		perror("sigprocmask error");
		exit(1);
	}
	while(1) {
		// use ctrl+z end process
		sigpending(&p);
		printsigset(&p);
		sleep(1);
	}
	return 0;
}