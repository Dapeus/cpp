#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

uint32_t uint2float(uint32_t u)
{
    if(u == 0)
        return 0;
    
    //get sign bit
    uint32_t s = u & 0x80000000;
    uint32_t f;
    uint32_t e;

    //remove sign bit
    u = u & 0x7fffffff;
    
    int n = 0;
    //remove the leading zero
    for(int i = 0; i < 32; i++) {
        if((u >> i) == 0x1) {
            n = i;
            break;
        }
    }

    if(n <= 23) {
        //no near
        f = u & (0xffffffff >> (33 - n)); //mask get f
        e = n + 127;

        printf("n:%x,f:%x,e:%d\n",n,f,e);
        return s | (e << 23) | f;
    } else {
        //near
        uint32_t x = (u >> (n - 23)) & 0x1;
        uint32_t g = (u >> (n - 23 - 1)) & 0x1;
        uint32_t r = (u >> (n - 23 - 2)) & 0x1;

        uint32_t s = 0;
        for(int j = 0; j < n-23-1; ++j) {
            s = (s | (u >> j)) & 0x1;
        }
    }
    return 0;
}

int main()
{
    //  0    0    b    7    8    0    0
    // 0000 0000 1101 0111 1000 0000 0000
    uint32_t u = 0x00b7800;
    uint2float(u);

    system("pause");
    return 0;
}