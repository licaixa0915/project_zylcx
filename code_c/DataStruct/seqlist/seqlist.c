#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DATESIZE 20

typedef struct
{
    int data[DATASIZE];
    int len;
}S_SEQLIST;

#define IN
#define OUT
#define INOUT

/*****************************************
 *函数名称:farsight_createSqeList
 *函数作用:创建一个空的顺序表
 *函数入参: void
 *函数出参: 无
 *函数返回值:S_SEQLIST * 返回申请内存的首地址
 *作者:licaixia
 *创建时间:2017年11月30日
 * ************************************/
S_SEQLIST* farsight_createSqeList(void)
{
    S_SEQLIST *pstSeqList = NULL;
    
    pstSeqList = (S_SEQLIST*)malloc(sizeof(S_SEQLIST));
    if (NULL == pstSeqList)
    {
        return NULL;
    }
    memset(pstSeqList, 0, sizeof(S_SEQLIST));

    return pstSeqList;
}


/*****************************************
 *函数名称:farsight_printSqeList
 *函数作用:打印顺序表中的所有的元素信息
 *函数入参: const S_SEQLIST *pstSeqList
 *函数出参: 无
 *函数返回值:void
 *作者:licaixia
 *创建时间:2017年11月30日
 * ************************************/
void farsight_printSqeList(IN const S_SEQLIST *pstSeqList)
{
    int i;

    printf("there is %d data in seqlist:\n", pstSeqList->len);
    
    for (i = 0; i < pstSeqList->len; i++)
    {
        printf("%d ", pstSeqList->data[i]);
    }
    printf("\n");
    return;
}


/*****************************************
 *函数名称:farsight_addDataSqeList
 *函数作用:将一个数据添加到顺序表的尾
 *函数入参: S_SEQLIST *pstSeqList 顺序表
 *          int num         需要添加的数
 *函数出参: S_SEQLIST *pstSeqList
 *函数返回值:int 返回添加结果，0表示成功 非0表示失败
 *作者:licaixia
 *创建时间:2017年11月30日
 * ************************************/
int farsight_addDataSqeList(INOUT S_SEQLIST *pstSeqList, IN int num)
{
    if (DATASIZE == pstSeqList->len)
    {
        printf("seqlist is full, add failed!\n");
        return -1;
    }

    pstSeqList->data[pstSeqList->len] = num;

    pstSeqList->len ++;

    return 0;
}

/*****************************************
 *函数名称:farsight_insertDataSqeList
 *函数作用:将一个数据插入到顺序表的中间位置
 *函数入参: S_SEQLIST *pstSeqList 顺序表
 *          int num         需要添加的数
 *          int pos         需要插入的位置   位置从0开始
 *函数出参: S_SEQLIST *pstSeqList
 *函数返回值:int 返回添加结果，0表示成功 -1表示失败
 *作者:licaixia
 *创建时间:2017年11月30日
 *************************************/
int farsight_insertDataSqeList
(
 INOUT S_SEQLIST *pstSeqList,
 IN    int num,
 IN    int pos
)
{
    int i;
    
    if (DATASIZE == pstSeqList->len)
    {
        printf("seqlist is full, insert failed!\n");
        return -1;
    }

    for (i = pstSeqList->len - 1; i >= pos; i--)
    {
        pstSeqList->data[i + 1] = pstSeqList->data[i];
    }

    pstSeqList->data[pos] = num;
    pstSeqList->len++;

    return 0;
}

int main(int argc, const char *argv[])
{
    S_SEQLIST *pstSeqList = NULL;

    pstSeqList = farsight_createSqeList();
    if (NULL == pstSeqList)
    {
        printf("create seqlist failed!\n");
        return 0;
    }
    farsight_addDataSqeList(pstSeqList, 10);
    farsight_addDataSqeList(pstSeqList, 20);
    farsight_addDataSqeList(pstSeqList, 5);
    farsight_addDataSqeList(pstSeqList, 6);
    farsight_addDataSqeList(pstSeqList, 2);
    farsight_addDataSqeList(pstSeqList, 8);
    farsight_insertDataSqeList(pstSeqList, 9, 1);
    farsight_printSqeList(pstSeqList);

    return 0;
}
