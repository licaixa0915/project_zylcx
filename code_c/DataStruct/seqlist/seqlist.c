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
 *��������:farsight_createSqeList
 *��������:����һ���յ�˳���
 *�������: void
 *��������: ��
 *��������ֵ:S_SEQLIST * ���������ڴ���׵�ַ
 *����:licaixia
 *����ʱ��:2017��11��30��
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
 *��������:farsight_printSqeList
 *��������:��ӡ˳����е����е�Ԫ����Ϣ
 *�������: const S_SEQLIST *pstSeqList
 *��������: ��
 *��������ֵ:void
 *����:licaixia
 *����ʱ��:2017��11��30��
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
 *��������:farsight_addDataSqeList
 *��������:��һ��������ӵ�˳����β
 *�������: S_SEQLIST *pstSeqList ˳���
 *          int num         ��Ҫ��ӵ���
 *��������: S_SEQLIST *pstSeqList
 *��������ֵ:int ������ӽ����0��ʾ�ɹ� ��0��ʾʧ��
 *����:licaixia
 *����ʱ��:2017��11��30��
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
 *��������:farsight_insertDataSqeList
 *��������:��һ�����ݲ��뵽˳�����м�λ��
 *�������: S_SEQLIST *pstSeqList ˳���
 *          int num         ��Ҫ��ӵ���
 *          int pos         ��Ҫ�����λ��   λ�ô�0��ʼ
 *��������: S_SEQLIST *pstSeqList
 *��������ֵ:int ������ӽ����0��ʾ�ɹ� -1��ʾʧ��
 *����:licaixia
 *����ʱ��:2017��11��30��
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
