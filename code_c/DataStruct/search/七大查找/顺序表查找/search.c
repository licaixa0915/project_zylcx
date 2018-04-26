#include <stdio.h>
#define DATANUM 500
/*1.顺序查找*/
int seq_search(int *pArr, int n, int key)
{
    int *p = pArr;
    int i = 0;

    pArr[n] = key;

    while (p[i] != key)
    {
        i++;
    }
    return i;
}

/*2.折半查找*/
int binary_search(int *pArr, int n, int key)
{
    int low,high,mid;

    low = 0;
    high = n - 1;
    while (low <= high)
    {
        mid = (low + high)/2;
        printf("%d ", mid+1);
        if (key < pArr[mid])
        {
            high = mid -1;
        }
        else if(key > pArr[mid])
        {
            low = mid + 1;
        }
        else
        {
            return mid;
        }
    }
    return n;
}

/*3.插值查找*/
int insert_search(int *pArr, int n, int key)
{
    int low,high,mid;

    low = 0;
    high = n - 1;
    while (low <= high)
    {
        mid =low + (high - low)*(key - pArr[low])/(pArr[high] - pArr[low]);
        printf("%d ", mid + 1);
        if (key < pArr[mid])
        {
            high = mid -1;
        }
        else if(key > pArr[mid])
        {
            low = mid + 1;
        }
        else
        {
            return mid;
        }
    }
    return n;
}
/*3.斐波那契查找*/
int fib_search(int *pArr, int n, int key)
{
    int low,high,mid;
    int F[20] = {0,1};
    int i;
    int k = 0;
    int dada = 0;

    /*1.构造斐波那契数列*/
    for (i = 2; i < 20; i++)
    {
        F[i] = F[i - 1] + F[i - 2];
    }

    low = 0;
    high = n - 1;
    /*2.计算n在斐波那契数列中的位置*/
    while (n > F[k] - 1)
    {
        k++;
    }
    //printf("k = %d\n", k);
    /*3.将数组按照F[k]个数进行补全*/
    for (i = n; i < F[k] - 1; i++)
    {
        pArr[i] = pArr[n-1];
    }

    while (low <= high)
    {
        mid =low + F[k-1] - 1;
        printf("%d ", mid + 1);
        if (key < pArr[mid])
        {
            high = mid -1;
            k = k - 1;
        }
        else if(key > pArr[mid])
        {
            low = mid + 1;
            k = k - 2;
        }
        else
        {
            if (mid <= high)
            {
                return mid;//查找成功
            }
            else
            {
                return -1;
            }
        }
    }
    return n;
}
int main(int argc, const char *argv[])
{
    //int arrData[DATANUM] = {1,2,5,6,8,10,16,20,25,26,28,29,30,60,100};
    int arrData[DATANUM] = {0};
    int pos = 0;
    int i = 0;
    int len = 0;

    for (i = 0; i < 100; i++)
    {
        /* code */
        arrData[i] = i+1;
    }
    len = i;

    printf("search by seq_search, i = %d\n", seq_search(arrData, len, 66));
    printf("search by binary_search, i = %d\n", binary_search(arrData, len, 66));
    printf("search by insert_search, i = %d\n", insert_search(arrData, len, 66));
    printf("search by fib_search, i = %d\n", fib_search(arrData, len, 66));

    return 0;
}
