// multiple thread calculate and test race condition

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

int s = 0;
pthread_mutex_t lock;

void* myfunc(void *argvs)
{
    // pthread_mutex_init(&lock, NULL);
    for(int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);
        s++;
        // printf("th:%s,%d\n", (char*)argvs,s);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main()
{
    pthread_t th1;
    pthread_t th2;

    pthread_create(&th1, NULL, myfunc, "1");
    pthread_create(&th2, NULL, myfunc, "0");

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("result: %d\n", s);
    return 0;
}
