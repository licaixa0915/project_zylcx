#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 10
typedef int datatype;
typedef struct stack
{
    datatype buf[N];
    int top;
}S_SEQSTACK;

/*1.创建空栈*/
S_SEQSTACK *farsight_createEmptyStack()
{
    S_SEQSTACK *pstStack = NULL;

    /*1.为顺序栈在堆上分配空间*/
    pstStack = (S_SEQSTACK*)malloc(sizeof(S_SEQSTACK));
    if (NULL == pstStack)
    {
        return NULL;
    }
    memset(pstStack, 0, sizeof(S_SEQSTACK));
    /*2.置为空栈*/
    pstStack->top = -1;

    /*3.返回顺序栈的首地址*/
    return pstStack;
}

/*2.顺序栈判空,就比较top和-1*/
int farsight_isEmptyStack(S_SEQSTACK *pstStack)
{
#if 0
    if (-1 == pstStack->top)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#endif
    return (-1 == pstStack->top)?1:0;
}

/*3.判断栈是不是满，比较top和N-1*/
int farsight_isFullStack(S_SEQSTACK *pstStack)
{
#if 0
    if (N-1 == pstStack->top)
    {
        return 1;
    }
    else
    {
        return 0;
    }
#endif
    return (N-1 == pstStack->top)?1:0;
}

/*4.元素进栈*/
void farsight_pushSeqStack(S_SEQSTACK *pstStack, datatype data)
{
    /*1.先移动top*/
    pstStack->top ++;
    /*2.再插入数据*/
    pstStack->buf[pstStack->top] = data;
    return;
}
/*5.元素出栈*/
datatype farsight_popSeqStack(S_SEQSTACK *pstStack)
{
    /*1.先取出栈顶元素*/
    datatype data = 0;
    data = pstStack->buf[pstStack->top];
    /*2.再移动top*/
    pstStack->top--;
    return data;
}
/*6.取出栈顶元素*/
datatype farsight_getTop(S_SEQSTACK *pstStack)
{
    return pstStack->buf[pstStack->top];
}
/*7.清空栈表*/
void farsight_clearStack(S_SEQSTACK* pstStack)
{
    free(pstStack);
    pstStack = NULL;
    return;
}

int main(int argc, const char *argv[])
{
    S_SEQSTACK *pstStack = NULL;
    int data = 0;

    pstStack = farsight_createEmptyStack();

    farsight_pushSeqStack(pstStack, 2);
    farsight_pushSeqStack(pstStack, 8);
    farsight_pushSeqStack(pstStack, 10);
    farsight_pushSeqStack(pstStack, 1);
    farsight_pushSeqStack(pstStack, 12);
    farsight_pushSeqStack(pstStack, 20);

    while (!farsight_isEmptyStack(pstStack))
    {
        data = farsight_popSeqStack(pstStack);
        printf("%d ", data);
    }
    farsight_clearStack(pstStack);
    printf("\n");
    return 0;
}
