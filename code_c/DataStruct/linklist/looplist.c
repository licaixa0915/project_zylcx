#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int datatype;
typedef struct node
{
    datatype data;
    struct node *pNext;
}S_LOOPNODE;

/*1.创建空的循环链表*/
S_LOOPNODE* farsight_createEmptyLoopList()
{
    S_LOOPNODE *pstHead = NULL;

    /*1.为头结点分配空间*/
    pstHead = (S_LOOPNODE*)malloc(sizeof(S_LOOPNODE));
    if (NULL == pstHead)
    {
        return NULL;
    }
    memset(pstHead, 0, sizeof(S_LOOPNODE));
    /*2.将头结点的指针域赋head*/
    pstHead->pNext = pstHead;
    /*3.返回头结点首地址*/

    return pstHead;
}

/*2.头插法插入新节点*/
int farsight_insertNodeToHead(S_LOOPNODE *pstHead, datatype data)
{
    S_LOOPNODE *pstNew = NULL;
   /* 1.为新节点在堆区分配空间，用指针pstNew来保存*/
    pstNew = (S_LOOPNODE*)malloc(sizeof(S_LOOPNODE));
    if (NULL == pstNew)
    {
        return -1;
    }
    memset(pstNew, 0, sizeof(S_LOOPNODE));
   /* 2.把data数据填充到temp的数据域  */
    pstNew->data = data;
   /* 3.在头结点之后插入数据
        <1>先让新节点的指针域保存头结点的指针域
        <2>再让头节点的指针与保存新节点的首地址*/
    pstNew->pNext = pstHead->pNext;
    pstHead->pNext = pstNew;
    return 0;
}

/*3.遍历循环列表的所有元素*/
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

/*4.清空循环链表 */
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
