#ifndef __API_H__
#define __API_H__

typedef  int datatype;

typedef struct quenode
{
    datatype data;//队列元素的数据
    struct quenode *pNext;//队列元素的next指针域
}S_QUEUE_NODE;
typedef struct 
{
    S_QUEUE_NODE *front;//front指向头结点的地址（链表中的head）
    S_QUEUE_NODE *rear;//rear指向尾结点的地址（即最后一个结点，数据域有值，指针域为NULL）
}S_LINK_QUEQUE;
S_LINK_QUEQUE *farsight_createEmptyLinkQueue();
int farsight_isEmpty(S_LINK_QUEQUE *pstLinkQueue);
int farsight_enterLinkQueue(S_LINK_QUEQUE *pstLinkQueue, datatype data);
int farsight_delLinkQueue(S_LINK_QUEQUE *pstLinkQueue);
void farsight_desdoryLinkQueue(S_LINK_QUEQUE *pstLinkQueue);

#define N 20
typedef struct stack
{
    datatype buf[N];
    int top;
}S_SEQSTACK;
S_SEQSTACK *farsight_createEmptyStack();
int farsight_isEmptyStack(S_SEQSTACK *pstStack);
int farsight_isFullStack(S_SEQSTACK *pstStack);
void farsight_pushSeqStack(S_SEQSTACK *pstStack, datatype data);
datatype farsight_popSeqStack(S_SEQSTACK *pstStack);
datatype farsight_getTop(S_SEQSTACK *pstStack);
void farsight_clearStack(S_SEQSTACK* pstStack);
#endif
