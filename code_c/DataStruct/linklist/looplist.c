#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int datatype;
typedef struct node
{
    datatype data;
    struct node *pNext;
}S_LOOPNODE;

/*1.�����յ�ѭ������*/
S_LOOPNODE* farsight_createEmptyLoopList()
{
    S_LOOPNODE *pstHead = NULL;

    /*1.Ϊͷ������ռ�*/
    pstHead = (S_LOOPNODE*)malloc(sizeof(S_LOOPNODE));
    if (NULL == pstHead)
    {
        return NULL;
    }
    memset(pstHead, 0, sizeof(S_LOOPNODE));
    /*2.��ͷ����ָ����head*/
    pstHead->pNext = pstHead;
    /*3.����ͷ����׵�ַ*/

    return pstHead;
}

/*2.ͷ�巨�����½ڵ�*/
int farsight_insertNodeToHead(S_LOOPNODE *pstHead, datatype data)
{
    S_LOOPNODE *pstNew = NULL;
   /* 1.Ϊ�½ڵ��ڶ�������ռ䣬��ָ��pstNew������*/
    pstNew = (S_LOOPNODE*)malloc(sizeof(S_LOOPNODE));
    if (NULL == pstNew)
    {
        return -1;
    }
    memset(pstNew, 0, sizeof(S_LOOPNODE));
   /* 2.��data������䵽temp��������  */
    pstNew->data = data;
   /* 3.��ͷ���֮���������
        <1>�����½ڵ��ָ���򱣴�ͷ����ָ����
        <2>����ͷ�ڵ��ָ���뱣���½ڵ���׵�ַ*/
    pstNew->pNext = pstHead->pNext;
    pstHead->pNext = pstNew;
    return 0;
}

/*3.����ѭ���б������Ԫ��*/
void farsight_printLoopList(S_LOOPNODE *pstHead)
{
    S_LOOPNODE *p = pstHead->pNext;

    while (p != pstHead)
    {
        printf("%d ", p->data);
        p = p->pNext;
    }

    printf("\n");
    return;
}

/*4.���ѭ������ */
void farsight_clearLoopList(S_LOOPNODE *pstHead)
{
    S_LOOPNODE *p = pstHead;
    S_LOOPNODE *d = NULL;

    while (p->pNext != pstHead)
    {
        d = p->pNext;
        p->pNext = d->pNext;
        free(d);
    }

    free(p);
    p = NULL;

    return;
}

int main(int argc, const char *argv[])
{
    S_LOOPNODE *pstHead = NULL;

    pstHead = farsight_createEmptyLoopList();
    if (NULL == pstHead)
    {
        printf("create list failed!\n");
        return 0;
    }

    farsight_insertNodeToHead(pstHead, 5);
    farsight_insertNodeToHead(pstHead, 6);
    farsight_insertNodeToHead(pstHead, 8);
    farsight_insertNodeToHead(pstHead, 1);
    farsight_insertNodeToHead(pstHead, 2);
    farsight_insertNodeToHead(pstHead, 10);
    farsight_printLoopList(pstHead);
    farsight_clearLoopList(pstHead);
    return 0;
}
