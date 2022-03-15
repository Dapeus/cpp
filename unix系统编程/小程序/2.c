//测量一个语句的大小，一个函数的大小，带参数函数的大小

#include<stdio.h>
#include<stdlib.h>

void t1(int l)
{
    if(l == 2)
        return;
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
}

void t2(int a, int b)
{

}

void t3(int a, int b)
{
    a = b;
}

void t4()
{

}

int main()
{
    int a1 = 1;
    printf("%p\n",&a1);
    printf("%p\n",t1);
    printf("%p\n",t2);
    printf("%p\n",t3);
    printf("%p\n",t4);
    //计算t4-t3,t3-t2即可测
    int l = 0;
    t1(l);

    int a2 = 0;
    printf("%p\n",&a2);
    return 0;
}
