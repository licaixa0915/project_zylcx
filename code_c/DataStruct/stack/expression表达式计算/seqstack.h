#ifndef __SEQSTACK_H__
#define __SEQSTACK_H__

#define N 20
typedef int datatype;
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
