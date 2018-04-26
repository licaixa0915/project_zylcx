#include <stdio.h>


void insertSort(int a[], int n)
{
    int i,j;
    int tmp = 0;

    for (i = 1; i < n; i++)
    {
        if (a[i] < a[i - 1])//与前边一个元素比较，前边是有序的，比前边小才需要插入
        {
            tmp = a[i];
            //找到合适的插入位置
            for (j = i-1; a[j] > tmp && j >= 0; j--)
            {
                a[j + 1] = a[j];
            }

            a[j + 1] = tmp;
        }
    }
    return;
}

int main(int argc, const char *argv[])
{
    int a[] = {5,6,1,2,10,20,8,9,3,5};
    int len = sizeof(a)/sizeof(a[0]);
    int i = 0;

    insertSort(a, len);
    for (i = 0; i < len; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
