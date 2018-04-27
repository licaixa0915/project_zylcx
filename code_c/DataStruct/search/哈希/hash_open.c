#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL_NODE 0
typedef int datatype;
typedef struct hash
{
    datatype *pData;//hash表
    int n;//保存hash表的保存数据的个数
}S_HASH;

/*1.创建hash表*/
S_HASH *createHashTable(int n)
{
    S_HASH *pstHash = NULL;

    pstHash = (S_HASH*)malloc(sizeof(S_HASH));
    if (NULL == pstHash)
    {
        printf("malloc error!\n");
        return NULL;
    }
    memset(pstHash, 0, sizeof(S_HASH));

    pstHash->pData = (datatype*)malloc(sizeof(datatype)*n);
    if (NULL == pstHash->pData)
    {
        printf("malloc hash table error!\n");
        free(pstHash);
        return NULL;
    }
    memset(pstHash->pData, NULL_NODE, sizeof(sizeof(datatype)*n));
    
    return pstHash;
}

/*2.根据hash函数插入数据*/
void insertData(S_HASH *pstHash, datatype key, int len)
{
    int k = 0;

    k = key%len;//构建哈希函数

    /*开放地址，往后找，找到空位置为止，由于%是数组的长度，也就是数据的个数，所以不会存在找不到的情况
     * 肯定都会找到，并且往后找，找到尾还没有就会返回到头继续找*/
    while(pstHash->pData[k] != NULL_NODE)
    {
        k = (k + 1) % len;
    }

    pstHash->pData[k] = key;//按照hash到的位置填充数据
    pstHash->n++;//维护n

    return;
}

/*3.查找数据,返回元素所在的数组下标*/
int searchData(S_HASH *pstHash, datatype key, int len)
{
    int k = key%len;

    while(pstHash->pData[k] != key)
    {
        k = (k+1)%len;

        if (k == key%len)//找了一圈没有找到，直接返回查找失败
        {
            return -1;
        }
    }

    return k;
}

/*4.遍历打印所有数据*/
void printHashTable(S_HASH *pstHash, int hashLen)
{
    int i = 0;

    for (i = 0; i < hashLen; i++)
    {
        printf("%d ", pstHash->pData[i]);
    }

    printf("\n");
    return;
}
int main(int argc, const char *argv[])
{
    int data[] = {12,67,56,16,25,37,22,29,15,47,48,34};
    int len = sizeof(data)/sizeof(data[0]);
    int i = 0;

    S_HASH *pstHash = NULL;
    pstHash = createHashTable(len);
    if (NULL == pstHash)
    {
        printf("create error!\n");
        return 0;
    }

    for (i = 0; i < len; i++)
    {
        insertData(pstHash, data[i], len);
    }

    printHashTable(pstHash, len);
    printf("find 48 index = %d\n", searchData(pstHash, 48, len));
    printf("find 5 index = %d\n", searchData(pstHash, 5, len));
    free(pstHash->pData);
    free(pstHash);
    return 0;
}
