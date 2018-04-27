#include <stdio.h>
#define MAX 100
#define PAIL_NUM 10
typedef struct pail
{
    int data[MAX];
    int n;
}S_PAIL;
/*获取数组中最大元素的位数*/
int getDigits(int a[], int n)
{
    int d = 1;
    int i;
    int p = 10;

    for (i = 0; i < n; i++)
    {
        if (a[i] > p)
        {
            p  = p * 10;
            d++;
        }
    }

    return d;
}
void radixSort(int a[], int len)
{
    int d = getDigits(a,len);
    int i,j,l;
    S_PAIL astPail[PAIL_NUM] = {0};
    int k = 1;
    int pos = 0;
    int loc = 0;

    for (i = 0; i < d; i++)//需要入桶d次
    {
        for (j = 0; j < len; j++)//入桶
        {
            pos = (a[j]/k)%PAIL_NUM;
            astPail[pos].data[astPail[pos].n] = a[j];
            astPail[pos].n++;
        }
        loc = 0;
        for (j = 0; j < PAIL_NUM; j++)//出桶
        {
            for (l = 0; l < astPail[j].n; l++)
            {
                a[loc] = astPail[j].data[l];
                loc++;
            }
            astPail[j].n = 0;//清空桶
        }
        
        k = k *10;
    }
    return;
}

int main(int argc, const char *argv[])
{
    int a[] = {288,52,123,30,212,10,233};
    int len = sizeof(a)/sizeof(a[0]);
    int i;

    radixSort(a, len);

    for (i = 0; i < len; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
