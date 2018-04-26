#include <stdio.h>

void heapAdjust(int a[], int i, int len)
{
    int j = 0;
    int k = i;

    for (j = 2*i; j <= len; j *= 2)
    {
        //找到跟节点j/2对应的两个字节点中较大的
        if ((j < len) && (a[j + 1] > a[j]))
            j++;
        if (a[k] < a[j])//需要交换
        {
            a[k] ^= a[j];
            a[j] ^= a[k];
            a[k] ^= a[j];
            k = j;
        }

    }
    return;
}

void heapSort(int a[], int len)
{
    int i;

    /*1.调整当前的完全二叉树，使之成为大顶端*/
    for (i = len/2; i >= 1; i--)
        heapAdjust(a, i, len);
    
    /*2.将堆顶的跟节点与最后难以个元素交换，然后重新调整完全二叉树*/
    for (i = len; i > 1; i--)
    {
        a[1] ^= a[i];
        a[i] ^= a[1];
        a[1] ^= a[i];
        heapAdjust(a, 1, i-1);
    }
    return;
}
int main(int argc, const char *argv[])
{
    int a[] = {0,16,7,3,20,17,8};
    //int a[] = {0,50,10,90,30,70,40,80,60,20};
    int len = sizeof(a)/sizeof(a[0]);
    int i = 0;

    heapSort(a, len - 1);
    for (i = 1; i < len; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}
