#include "foo.h"

int find_max(int *arr, int n)
{
    int max = 0;
    for(int i = 0; i < n; i++) {
        if(arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}