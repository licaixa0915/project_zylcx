#include <stdio.h>

void quickSort(int a[], int low, int high)
{
    int i = low;
    int j = high;
    int key = a[low];

    while (i < j)
    {
        while ((i < j) && (a[j] >= key))
        {
            j--;
        }
        //��keyС���������
        if (i < j)
            a[i] = a[j];
        while ((i < j) && (a[i] <= key))
        {
            i++;
        }
        //��key�󣬷����ұ�
        if (i < j)
            a[j] = a[i];
    }
    a[i] = key;

    //�ݹ�ֳɵ�������������
    if (i - 1 > low)//����Ԫ�ظ�������1
        quickSort(a, low, i-1);
    if (high > i + 1)
        quickSort(a, i+1, high);
    return;
}

int main(int argc, const char *argv[])
{
    int a[] = {49,38,65,97,76,13,27,49};
    int len = sizeof(a)/sizeof(a[0]);
    int i;

    quickSort(a, 0, len-1);
    for (i = 0; i < len; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
