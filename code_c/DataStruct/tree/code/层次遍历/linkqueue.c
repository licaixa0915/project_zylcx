#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

/*1.创建空队列*/
S_LINK_QUEQUE *farsight_createEmptyLinkQueue()
{
    S_LINK_QUEQUE *pstLinkQueue = NULL;
    S_QUEUE_NODE *pstQueHead = NULL;

    pstLinkQueue = (S_LINK_QUEQUE*)malloc(sizeof(S_LINK_QUEQUE));
    if (NULL == pstLinkQueue)
    {
        return NULL;
    }
    
    memset(pstLinkQueue, 0, sizeof(S_LINK_QUEQUE));
    /*创建队列的头节点*/
    pstQueHead = (S_QUEUE_NODE*)malloc(sizeof(S_QUEUE_NODE));
    if (NULL == pstQueHead)
    {
        free(pstLinkQueue);
        return NULL;
    }
    memset(pstQueHead,0, sizeof(S_QUEUE_NODE));

    /*初始front指针和rear指针*/
    pstLinkQueue->front = pstQueHead;
    pstLinkQueue->rear = pstQueHead;

    /*返回队列头指针*/
    return pstLinkQueue;
}
/*2.判断队列是否为空队列*/
int farsight_isEmpty(S_LINK_QUEQUE *pstLinkQueue)
{
    if (pstLinkQueue->front == pstLinkQueue->rear)
    {
        return 1;
    }

    return 0;
}
/*3.入队列*/
int farsight_enterLinkQueue(S_LINK_QUEQUE *pstLinkQueue, datatype data)
{
    S_QUEUE_NODE *pstNewNode = NULL;

    /*为新节点在堆上分配空间*/
    pstNewNode = (S_QUEUE_NODE*)malloc(sizeof(S_QUEUE_NODE));
    if (NULL == pstNewNode)
    {
        return -1;
    }

    memset(pstNewNode, 0, sizeof(S_QUEUE_NODE));

    /*将数据填充到分配的内存中*/
    pstNewNode->data = data;

    /*将新节点放到队尾，相当于尾插法加入到链表中*/
    pstNewNode->pNext = NULL;
    pstLinkQueue->rear->pNext = pstNewNode;
    pstLinkQueue->rear = pstNewNode;

    return 0;
}
/*4.出队列*/
datatype farsight_delLinkQueue(S_LINK_QUEQUE *pstLinkQueue)
{
    S_QUEUE_NODE *pstDelNode = NULL;
    int data = 0;

    /*1.保存头节点的指针*/
    pstDelNode = pstLinkQueue->front;

    /*2.更新头节点的next域*/
    pstLinkQueue->front = pstDelNode->pNext;
    free(pstDelNode);
    return pstLinkQueue->front->data;
}

/*5.销毁队列*/
void farsight_desdoryLinkQueue(S_LINK_QUEQUE *pstLinkQueue)
{
    S_QUEUE_NODE *p = pstLinkQueue->front;
    S_QUEUE_NODE *d = NULL;

    while (p)
    {
        d = p;
        free(d);
        p = p->pNext;
    }
    free(pstLinkQueue);
    return;
}
#if 0
int main(int argc, const char *argv[])
{
    S_LINK_QUEQUE *pstLinkQueue = NULL;

    pstLinkQueue = farsight_createEmptyLinkQueue();
    if (NULL == pstLinkQueue)
    {
        printf("create failed!\n");
        return 0;
    }

    farsight_enterLinkQueue(pstLinkQueue, 1);
    farsight_enterLinkQueue(pstLinkQueue, 2);
    farsight_enterLinkQueue(pstLinkQueue, 3);
    farsight_enterLinkQueue(pstLinkQueue, 4);
    farsight_enterLinkQueue(pstLinkQueue, 5);
    farsight_enterLinkQueue(pstLinkQueue, 6);

    while (!farsight_isEmpty(pstLinkQueue))
    {
        printf("%d ", farsight_delLinkQueue(pstLinkQueue));
    }

    printf("\n");
    farsight_desdoryLinkQueue(pstLinkQueue);
    return 0;
}
#endif
