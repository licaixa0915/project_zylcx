#include <stdio.h>
#include "api.h"

#define BALLNUM 27

int farsight_isOriginalBall(S_LINK_QUEQUE *pstBall)
{
    int i = 0;
    S_QUEUE_NODE *p = pstBall->front->pNext;
    for (i = 1; i <= BALLNUM; i++)
    {
        if ((p)&&(p->data != i))
        {
            return 0;
        }
        p = p->pNext;
    }
    return 1;
}

int farsight_ballClock()
{
    S_LINK_QUEQUE *pstBall = NULL;//球队列
    S_SEQSTACK  *pstMinStack = NULL;//分钟栈
    S_SEQSTACK  *pstMin5Stack = NULL;//5分钟栈
    S_SEQSTACK  *pstHourStack = NULL;//小时栈
    int halfDays = 0;
    pstBall = farsight_createEmptyLinkQueue();
    if (NULL == pstBall)
    {
        printf("create queue failed!\n");
        return 0;
    }

    pstMinStack = farsight_createEmptyStack();
    if (NULL == pstMinStack)
    {
        printf("create min stack failed!\n");
        /*这里return出去，一定记得释放上边的内存*/
        farsight_desdoryLinkQueue(pstBall);
        return 0;
    }
    pstMin5Stack = farsight_createEmptyStack();
    if (NULL == pstMin5Stack)
    {
        printf("create 5min stack failed!\n");
        /*这里return出去，一定记得释放上边的内存*/
        farsight_desdoryLinkQueue(pstBall);
        farsight_clearStack(pstMinStack);
        return 0;
    }
    pstHourStack = farsight_createEmptyStack();
    if (NULL == pstHourStack)
    {
        printf("create hour stack failed!\n");
        /*这里return出去，一定记得释放上边的内存*/
        farsight_desdoryLinkQueue(pstBall);
        farsight_clearStack(pstMinStack);
        farsight_clearStack(pstMin5Stack);
        return 0;
    }

    /*1.将球队列放入27个球 00:00*/
    int i = 1;
    int ball = 0;
    for (i = 1; i <= BALLNUM; i++)
    {
        farsight_enterLinkQueue(pstBall, i);
    }

    while (1)
    {
        /*2.一个球出队列*/
        ball = farsight_delLinkQueue(pstBall);

        /*3.入分钟栈*/
        /*top是从-1开始，先增加top再入栈的，所以top+1等于栈中元素的个数*/
        if (pstMinStack->top + 1 < 4)
        {
            farsight_pushSeqStack(pstMinStack, ball);
            continue;
        }

        /*4.走到这里说名是分钟的栈满了，是第5个球来了*/
        /*   此时需要将第五个球入5分钟栈，将分钟栈中的球全部出栈入球队列*
         *   /
         /*先将球全部出栈入队列*/
        while (!farsight_isEmptyStack(pstMinStack))
        {
            farsight_enterLinkQueue(pstBall, 
                                  farsight_popSeqStack(pstMinStack));
        }
        /*处理5分钟栈，跟处理上边分钟栈的方式一样*/
        if (pstMin5Stack->top + 1 < 11)
        {
            farsight_pushSeqStack(pstMin5Stack, ball);
            continue;
        }

        /*5. 5分钟栈满了，出栈入队列*/
        while (!farsight_isEmptyStack(pstMin5Stack))
        {
            farsight_enterLinkQueue(pstBall, 
                                  farsight_popSeqStack(pstMin5Stack));
        }
        /*处理小时栈，跟处理上边分钟栈的方式一样*/
        if (pstHourStack->top + 1 < 11)
        {
            farsight_pushSeqStack(pstHourStack, ball);
            continue;
        }
        
        while (!farsight_isEmptyStack(pstHourStack))
        {
            farsight_enterLinkQueue(pstBall, 
                                  farsight_popSeqStack(pstHourStack));
        }
        /*6.hour栈中第12个球来了，直接入队列*/
        farsight_enterLinkQueue(pstBall, ball);

        /*走到这里，所有的球又都回到球队列中了，花了半天的时间*/
        halfDays++;

        /*判断当前球队列是不是最初状态，是则返回，不是继续*/
        if (farsight_isOriginalBall(pstBall))
        {
            break;
        }
    }
    return halfDays/2;

}


int main(int argc, const char *argv[])
{
    int days = 0;

    days = farsight_ballClock();
    if (0 == days)
    {
        printf("get days failed!\n");
        return 0;
    }

    printf("days = %d\n", days);
    return 0;
}
