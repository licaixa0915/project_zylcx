#include <stdio.h>


void shellSort(int a[], int n)
{
    int i,j,k;
    int tmp = 0;

    for (k = n/2; k >= 1; k/=2)
    {
        for (i = k; i < n; i++)
        {
            if (a[i] < a[i - k])//与前边对应步长元素比较，前边是有序的，比前边小才需要插入
            {
                tmp = a[i];
                //找到合适的插入位置
                for (j = i-k; a[j] > tmp && j >= 0; j -= k)
                {
                    a[j + k] = a[j];
                }

                a[j + k] = tmp;
            }
        }
    }
    return;
}

int main(int argc, const char *argv[])
{
    int a[] = {5,6,1,2,10,20,8,9,3,5};
    int len = sizeof(a)/sizeof(a[0]);
    int i = 0;

    shellSort(a, len);
    for (i = 0; i < len; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
