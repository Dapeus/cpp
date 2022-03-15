#include <stdio.h>
#include "bar.h"
#include "foo.h"
  
int main()
{
    int arr[5] = {1, 3, 5, 7, 9};
    int max = find_max(arr,5);
    int min = find_min(arr,5);
    printf("max:%d\n",max);
    printf("min:%d\n",min);
    
    return 0;
}