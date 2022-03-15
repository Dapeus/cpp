// multiple thread calculate add

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

typedef struct {
    int start;
    int end;
    int result;
} CALCULATE_UNIT;

void* myfunc(void *argvs)
{
    for(int i = ((CALCULATE_UNIT *)argvs)->start; i < ((CALCULATE_UNIT *)argvs)->end; i++) {
        ((CALCULATE_UNIT *)argvs)->result += rand()%5;
        printf("%d\n", i);
    }
    return NULL;
}

int main()
{
    pthread_t th1;
    pthread_t th2;

    CALCULATE_UNIT calu1 = {0, 10000, 0};
    CALCULATE_UNIT calu2 = {10000, 20000, 0};
    
    pthread_create(&th1, NULL, myfunc, &calu1);
    pthread_create(&th2, NULL, myfunc, &calu2);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    printf("result1: %d\n",calu1.result);
    printf("result2: %d\n",calu2.result);
    return 0;
}
