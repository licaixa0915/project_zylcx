#ifndef __API_H__
#define __API_H__

typedef char Datatype;
typedef struct bitreenode
{
    Datatype data;//�ڵ�����
    struct bitreenode *pstLChild;//���ӽڵ�ָ��
    struct bitreenode *pstRChild;//�Һ��ӽڵ�ָ��
}S_BITREE_NODE;

typedef  S_BITREE_NODE* datatype;
typedef struct quenode
{
    datatype data;//����Ԫ�ص�����
    struct quenode *pNext;//����Ԫ�ص�nextָ����
}S_QUEUE_NODE;
typedef struct 
{
    S_QUEUE_NODE *front;//frontָ��ͷ���ĵ�ַ�������е�head��
    S_QUEUE_NODE *rear;//rearָ��β���ĵ�ַ�������һ����㣬��������ֵ��ָ����ΪNULL��
}S_LINK_QUEQUE;
S_LINK_QUEQUE *farsight_createEmptyLinkQueue();
int farsight_isEmpty(S_LINK_QUEQUE *pstLinkQueue);
int farsight_enterLinkQueue(S_LINK_QUEQUE *pstLinkQueue, datatype data);
datatype farsight_delLinkQueue(S_LINK_QUEQUE *pstLinkQueue);
void farsight_desdoryLinkQueue(S_LINK_QUEQUE *pstLinkQueue);
#if 0
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
#endif
