//打印各种类型的数据及其指针

#include<stdio.h>
#include<stdlib.h>

int main()
{
    char a = 'a';
    int b = 1;
    unsigned int c = 2;
    float d = 0.1;
    double e = 0.2;

    char *pa = &a;
    int *pb = &b;
    unsigned int *pc = &c;
    float *pd = &d;
    double *pe = &e;

    printf("%c\t\t%p\n",a,&a);
    printf("%p\t%p\n",pa,&pa);

    printf("%d\t\t%p\n",b,&b);
    printf("%p\t%p\n",pb,&pb);

    printf("%u\t\t%p\n",c,&c);
    printf("%p\t%p\n",pc,&pc);

    printf("%f\t%p\n",d,&d);
    printf("%p\t%p\n",pd,&pd);

    printf("%lf\t%p\n",e,&e);
    printf("%p\t%p\n",pe,&pe);

    return 0;
}