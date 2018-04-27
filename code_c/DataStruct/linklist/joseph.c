#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*ѭ���������֮ǰ��������Ļ����ϵģ����Դ���ֱ�������ù�������
 *ԭ���Ļ������޸�һ�¾Ϳ����ˡ�
 * */
typedef int datatype;
typedef struct node
{
    datatype data;         //�����������
    struct node *pNext;    //�����ָ����ָ����һ���ڵ�
}S_LINKNODE;

/*****************************************
 *��������:farsight_createEmptyLinkList
 *��������:����һ��������,����ͷ�ڵ��ָ��
 *�������:void 
 *��������:void
 *��������ֵ:S_LINKNODE * ͷ�ڵ�ָ��
 *����:licaixia
 *����ʱ��:2017��12��1��
 * ************************************/
S_LINKNODE *farsight_createEmptyLinkList()
{
    S_LINKNODE *pstLinkHead = NULL;

    /*1.Ϊͷ�ڵ����ռ�*/
    pstLinkHead = (S_LINKNODE*)malloc(sizeof(S_LINKNODE));
    if (NULL == pstLinkHead)
    {
        return NULL;
    }
    memset(pstLinkHead, 0, sizeof(S_LINKNODE));
    /*2.��ʼ��ͷ�ڵ���������ָ����*/
    pstLinkHead->pNext = NULL;
    /*3.��ͷ�ڵ��ָ�뷵�س�ȥ */
    return pstLinkHead;
}

/*��ӡ�����е����е�Ԫ��*/
void farsight_printLinkList(S_LINKNODE *pstLinkHead)
{
    S_LINKNODE *p = pstLinkHead;

    if (NULL == p)
    {
        printf("this list is empty!\n");
        return;
    }

    printf("there is %d num in list\n", p->data);
    while(NULL != p->pNext)
    {
        printf("%d ", p->pNext->data);
        p = p->pNext;
    }
    printf("\n");
    return;
}

/*ͷ�巨����ڵ�*/
int farsight_addNodeToHead(S_LINKNODE *pstLinkHead, datatype data)
{
    /*˼��:֮ǰ˳������Ԫ�ص�ʱ���жϱ��ǲ������ˣ�������û������ʱ��
     *�𰸣���Ȼû���ˣ�������Ԫ��ʧ���ˣ��ǿ϶��������ڴ��ʧ���ˣ���ʱ�򣬾Ͳ������Ԫ��ʧ�ܵ������ˣ��Ǿ�Ҫ崻���*/
    S_LINKNODE *pstLinkNode = NULL;

    /* 1.Ϊͷ�ڵ����ռ� */
    pstLinkNode = (S_LINKNODE*)malloc(sizeof(S_LINKNODE));
    if (NULL == pstLinkNode)
    {
        printf("malloc failed~\n");
        return -1;
    }
    /* 2.��data��䵽ͷ�ڵ���*/
    pstLinkNode->data = data;
    /* 3.��ӽڵ㵽ͷ�ڵ���
     *    3.1 ���½ڵ��ָ����ָ��֮ǰͷ�ڵ�֮����Ǹ�Ԫ��
     *    3.2 ��ͷ�ڵ��ָ����ָ���½ڵ�*/
    pstLinkNode->pNext = pstLinkHead->pNext;
    pstLinkHead->pNext = pstLinkNode;
    pstLinkHead->data++;
    return 0;
}
/*β�巨����ڵ�*/
int farsight_addNodeToTail(S_LINKNODE *pstLinkHead, datatype data)
{
    S_LINKNODE *pstLinkNode = NULL;
    S_LINKNODE *p = NULL;

    /*(1) Ϊͷ�ڵ����ռ�*/
    pstLinkNode = (S_LINKNODE*)malloc(sizeof(S_LINKNODE));
    if (NULL == pstLinkNode)
    {
        return -1;
    }
    memset(pstLinkNode, 0, sizeof(S_LINKNODE));

    /*(2) ���data��*/
    pstLinkNode->data = data;
    
    /*(3) ������������β�ͣ����ڵ���뵽β�ڵ��
     *    3.1 β�ڵ��ָ����ָ���½ڵ�
     *    3.2 �½ڵ�ָ������NULL
     * */
    p = pstLinkHead;
    while(NULL != p->pNext)
    {
        p = p->pNext;
    }

    p->pNext = pstLinkNode;
    pstLinkNode->pNext = NULL;
    pstLinkHead->data++;

    return 0;
}
/*ɾ��ĳ��Ԫ�� */
int farsight_delNode(S_LINKNODE *pstLinkHead, datatype data)
{
    S_LINKNODE *p = pstLinkHead;
    S_LINKNODE *pstDel = NULL;

    while(NULL != p->pNext)
    {
        if (p->pNext->data == data)
        {
            break;
        }
        p = p->pNext;
    }

    if (NULL == p->pNext)
    {
        return -1;
    }

    pstDel = p->pNext;
    p->pNext = pstDel->pNext;

    free(pstDel);
    pstLinkHead->data--;

    return 0;
}
/*������������� */
void farsight_sort(S_LINKNODE *pstLinkHead)
{
    S_LINKNODE *p = NULL;

    datatype tmp = 0;
    int i,j;

    for (i = 0; i < pstLinkHead->data - 1; i++)
    {
        p = pstLinkHead->pNext;

        for (j = 0; j < pstLinkHead->data - 1 - i; j++)
        {
            if (p->data > p->pNext->data)
            {
                tmp = p->data;
                p->data = p->pNext->data;
                p->pNext->data = tmp;

            }
            p = p->pNext;
        }

    }
    
    return;
}

/*�����½ڵ㵽�������*/
int farsight_insertNodeToOderedList(S_LINKNODE *pstLinkHead, datatype data)
{
    S_LINKNODE *pstLinkNode = NULL;
    S_LINKNODE *p = pstLinkHead;

    /*(1)  Ϊ�½�����ռ䣬��temp�����׵�ַ*/
    pstLinkNode = (S_LINKNODE*)malloc(sizeof(S_LINKNODE));
    if (NULL == pstLinkNode)
    {
        return -1;
    }
    memset(pstLinkNode, 0, sizeof(S_LINKNODE));

    /*(2)  ���������ݴ�������½��������*/
    pstLinkNode->data = data;
    
    /* (3)  ѭ���ҵ�����λ��ǰһ���ڵ�,p��*/
    /* ע��pָ��ǰ�ڵ㣬�����ǱȽ��Ǻ�һ���ڵ������p->next-data*/
    while (NULL != p->pNext)
    {
        if (p->pNext->data > data)
        {
            break;
        }
        p = p->pNext;
    }
    /*(4)  ��p�ڵ�ĺ���������ݡ�*/
    /*<1>�����½ڵ��ָ���򱣴�p�ڵ��ָ����*/
    /*<2>p��ָ�����������½����׵�ַ��*/
    pstLinkNode->pNext = p->pNext;
    p->pNext = pstLinkNode;
    pstLinkHead->data++;
    return 0;
}
/*�������*/
void farsight_clearLinkList(S_LINKNODE **pstLinkHead)
{
    S_LINKNODE *p = *pstLinkHead;
    S_LINKNODE *q = NULL;

    while(NULL != p)
    {
        q = p;
        p = p->pNext;
        free(q);
    }
    *pstLinkHead = NULL;
    return;

}

/*�����е�����Ԫ������*/
void farsight_reverseLinkList(S_LINKNODE* pstLinkHead)
{
    /*(1) ����ָ�����p�������һ����Ч���ݽ��ĵ�ַ��*/
    S_LINKNODE *p = pstLinkHead->pNext;
    S_LINKNODE *q = NULL;
    
    /*(2) ����ͷ�ڵ���Ч���ݽ���ָ������ΪNULL��*/
    pstLinkHead->pNext = NULL;
    while (NULL != p)
    {
        /*(3) ��p��ʼ����������q����p�������뵽ͷ����*/
        q = p;
        p = p->pNext;
        q->pNext = pstLinkHead->pNext;
        pstLinkHead->pNext = q;
    }
    return;
}
/* ��������������кϲ� */
void farsight_mergeTwoLinkList(S_LINKNODE *pstLinkListA, S_LINKNODE*pstLinkListB)
{
    S_LINKNODE *p = pstLinkListA->pNext;
    S_LINKNODE *pPre = pstLinkListA;
    S_LINKNODE *q = pstLinkListB->pNext;

    while (NULL != p && NULL != q)
    {
        if (p->data >= q->data)
        {
            pPre->pNext = q;
            q = q->pNext;
            pPre->pNext->pNext = p;
        }
        pPre = p;
        p = p->pNext;
    }

    if (NULL == p && NULL != q)
    {
        p =q;    
    }

    return;
}
/*�ж��������Ƿ��л�*/
int farsight_judgeRingInLinkList(S_LINKNODE*pstLinkHead)
{
    S_LINKNODE *pstSlow = pstLinkHead;
    S_LINKNODE *pstFast = pstLinkHead;


    while (pstSlow != NULL && pstFast->pNext != NULL)
    {
        pstSlow = pstSlow->pNext;
        pstFast = pstFast->pNext->pNext;
        if (pstSlow == pstFast)
        {
            return 1;
        }
    }

    return 0;
}

/*��ԭ���ĵ�����������ѭ������*/
void farsight_createLoopLinkList(S_LINKNODE *pstLinkHead, S_LINKNODE **pstTailNode)
{
    S_LINKNODE *p = pstLinkHead;
    /*1.ѭ��֮ǰ�ĵ��������ҵ�β�ڵ�,����β�ڵ�ָ��*/
    while (p->pNext)
    {
        p = p->pNext;
    }
    *pstTailNode = p;
    /*2.��β�ڵ��ָ��ָ��ͷ�ڵ��ĵ�һ����������Ľڵ�*/
    (*pstTailNode)->pNext = pstLinkHead->pNext;
    /*3.�ͷ�ͷ�ڵ�*/
    free(pstLinkHead);
    return;
}

/*���ѭ���������������*/
void farsight_printLoopList(S_LINKNODE *pstTailNode)
{
    S_LINKNODE *p = pstTailNode->pNext;

    while(p != pstTailNode)
    {
        printf("%d ", p->data);
        p = p->pNext;
    }
    printf("%d", p->data);
    putchar('\n'); 
    return;
}
void joseph(int n, int k, int m)
{
    int i;
    S_LINKNODE *pstHead = NULL;
    S_LINKNODE *pstTail = NULL;
    S_LINKNODE *p = NULL;
    S_LINKNODE *d = NULL;

    pstHead = farsight_createEmptyLinkList();
    if (NULL == pstHead)
    {
        printf("create failed!\n");
        return;
    }
    
    for (i = 1; i <= n; i++)
    {
        farsight_addNodeToTail(pstHead, i);
    }

    farsight_createLoopLinkList(pstHead, &pstTail);
    p = pstTail->pNext;

    for (i = 0; i < k - 1; i++)
    {
        p = p->pNext;
    }
    
    while (p != p->pNext)
    {
        for (i = 0; i < m - 2; i++)
        {
            p = p->pNext;
        }
        printf("%d ", p->pNext->data);
        d = p->pNext;
        p->pNext = p->pNext->pNext;
        p = p->pNext;

        free(d);
    }

    printf("%d\n", p->data);
    free(p);
    p = NULL;
    return;
}

int main(int argc, const char *argv[])
{
    int n,k,m;

    scanf("%d%d%d", &n, &k, &m);
    joseph(n,k,m);
    return 0;
}
