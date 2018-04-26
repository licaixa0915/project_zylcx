#include <stdio.h>
void swap(int *a, int *b)
{
    int tmp = 0;

    tmp = *a;
    *a = *b;
    *b = tmp;

    return;
}
void selectSort(int *pszData, int n)
{
    int i, j;
    int min;

    for (i = 0; i < n; i++)
    {
        min = i;
    
        for (j = i + 1; j < n; j++)
        {
            if (pszData[j] < pszData[min])
            {
                min = j;
            }
        }

        if (min != i)
        {
            swap(&pszData[i], &pszData[min]);            
        }
    }
    return;
}

int main(int argc, const char *argv[])
{
    int data[] = {5,6,1,2,10,20,8,9,3,5};
    int len = sizeof(data)/sizeof(data[0]);
    int i;

    selectSort(data, len);
    for (i = 0; i < len; i++)
    {
        printf("%d ", data[i]);
    }
    printf("\n");
    return 0;
}
