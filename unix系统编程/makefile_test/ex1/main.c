#include <stdio.h>
#include "tool.h"

int main()
{
    int arr[5] = {1, 3, 5, 7, 9};
    int max = find_max(arr,5);
    printf("%d\n",max);

    return 0;
}