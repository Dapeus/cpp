#include <stdio.h>
#include "bar.h"
#include "foo.h"
  
int main()
{
    int arr[5] = {1, 3, 5, 7, 9};
    int min = find_min(arr,5);
    printf("min:%d\n",min);
    
    return 0;
}