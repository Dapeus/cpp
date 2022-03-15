#include<unistd.h>
#include<stdio.h>
#include<string.h>

int main()
{
    char s[100] = "i love me do i know";
    const char *delimiter = " ";
    char *t;
    int i = 0;
    if((t = strtok(s, delimiter)) != NULL) {
        printf("%s\n",t);
        for(i = 1; (t = strtok(NULL, delimiter)) != NULL; i++) {
            printf("%s\n",t);
        }
    }
    printf("%d\n", i);
    return 0;
}