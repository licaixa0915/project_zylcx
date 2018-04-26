#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int datatype;
#define MAX 10
#define NULL_NODE 0
typedef struct node 
{
    datatype data;
    struct node *pNext;
}S_HASH_NODE;

/*1.创建一个空的hash表，返回hash表的首地址*/
S_HASH_NODE *createHashTable(int len)
{
    S_HASH_NODE *pstHash = NULL;

    pstHash = (S_HASH_NODE*)malloc(sizeof(S_HASH_NODE)*len);
    if (NULL == pstHash)
    {
        printf("malloc failed~\n");
        return NULL;
    }

    memset(pstHash, 0, sizeof(S_HASH_NODE)*len);
    return pstHash;
}

/*2.插入数据到hash表中*/
void insertData(S_HASH_NODE *pstHash, datatype key, int len)
{
    int k = key%len;
    S_HASH_NODE *pstNewNode = NULL;
    S_HASH_NODE *pstNode = NULL;

    /*hash表中对应的没有数据，直接放到hash表中即可*/
    if (NULL_NODE == pstHash[k].data)
    {
        pstHash[k].data = key;
    }
    else//需要往下挂链，这时候需要重新申请节点信息
    {
        pstNewNode = (S_HASH_NODE*)malloc(sizeof(S_HASH_NODE));
        if (NULL == pstNewNode)
        {
            printf("malloc error");
            return;
        }
        memset(pstNewNode, 0, sizeof(S_HASH_NODE));
        pstNewNode->data = key;
        /*找到相应的位置，挂上去*/
        pstNode = &pstHash[k];

        while(pstNode->pNext != NULL)
        {
            pstNode = pstNode->pNext;
        }

        pstNode->pNext = pstNewNode;
    }

    return;
}
/*3.打印表中所有数据*/
void printfAll(S_HASH_NODE *pstHash, int len)
{
    int i;
    S_HASH_NODE*pstNode = NULL;

    for (i = 0; i < len; i++)
    {
        if (pstHash[i].data)
        {
            printf("%d ", pstHash[i].data);
        }
        
        pstNode = pstHash[i].pNext;
        while(pstNode)
        {
            printf("%d ", pstNode->data);
            pstNode = pstNode->pNext;
        }
    }
    return;
}
/*查找数据*/
int searchData(S_HASH_NODE *pstHash, datatype key, int len)
{
    int i;
    S_HASH_NODE*pstNode = NULL;
    int k = key%len;

    if (pstHash[k].data == key)
    {
        return k;
    }

    pstNode = pstHash[k].pNext;
    while (pstNode)
    {
        if (pstNode->data == key)
        {
            return k;
        }
        pstNode = pstNode->pNext;
    }

    return -1;
}
int main(int argc, const char *argv[])
{
    int data[] = {12,67,56,16,25,37,22,29,15,47,48,34};
    int len = sizeof(data)/sizeof(data[0]);
    int i = 0;
    S_HASH_NODE *pstHash = NULL;

    pstHash = createHashTable(len);
    if (NULL == pstHash)
    {
        printf("create failed!\n");
        return 0;
    }

    for (i = 0; i < len; i++)
    {
        insertData(pstHash, data[i], len);
    }
    printfAll(pstHash, len);
    printf("\n");
    printf("search data 56 index is %d\n", searchData(pstHash, 56,len));
    printf("search data 50 index is %d\n", searchData(pstHash, 50,len));
    return 0;
}
