#include <stdio.h>
void swap(int *a, int *b)
{
    int tmp = 0;

    tmp = *a;
    *a = *b;
    *b = tmp;

    return;
}
void bubbleSort(int *pszData, int n)
{
    int i, j;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - 1 - i; j++)
        {
            if (pszData[j] > pszData[j+1])
            {
                swap(&pszData[j], &pszData[j+1]);            
            }
        }
    }
    return;
}
/*”≈ªØ√∞≈›≈≈–ÚÀ„∑®*/
void bubbleSort_1(int* pszData, int n)
{
    int i,j;
    int flag = 1;

    for (i = 0; i < n - 1 && flag; i++)
    {
        flag =  0;
        for (j = 0; j < n - 1 - i; j++)
        {
            if (pszData[j] > pszData[j+1])
            {
                swap(&pszData[j], &pszData[j+1]);
                flag = 1;
            }
        }
    }
    return;
}

int main(int argc, const char *argv[])
{
    int data[] = {5,6,1,2,10,20,8,9,3,5};
    int len = sizeof(data)/sizeof(data[0]);
    int i;

    bubbleSort_1(data, len);
    for (i = 0; i < len; i++)
    {
        printf("%d ", data[i]);
    }
    printf("\n");
    return 0;
}
