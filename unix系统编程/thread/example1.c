#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void* myfunc(void *argvs)
{
    for(int i = 0; i < 500; i++) {
        printf("%d\n",i);
    }
}

int main()
{
    pthread_t th1;
    pthread_t th2;

    pthread_create(&th1, NULL, myfunc, NULL);
    pthread_create(&th2, NULL, myfunc, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    return 0;
}
