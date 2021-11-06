#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int lowBit(int num)
{
    return log(num & ((~num) + 1))/log(2);
}

unsigned letter(unsigned num)
{
    unsigned x1 = 0x22222222 & num;
    unsigned x2 = 0x44444444 & num;
    unsigned x3 = 0x88888888 & num;
    return (x3 >> 3) & ((x2 >> 2) | (x1 >> 1));
}

int main()
{
    unsigned a = 0xffff2222;
    printf("%d\n",lowBit(a));
    printf("0x%x letter is 0x%x\n",0xabcdefab,letter(0xabcdefab));
    printf("0x%x letter is 0x%x\n",0x11111111,letter(0x11111111));
    system("pause");
    return 0;
}
