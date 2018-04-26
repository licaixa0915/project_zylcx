#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 21
typedef int datatype;
/*定义队列的结构体*/
typedef struct queue
{
    datatype buf[MAXSIZE];
    int front;//保存数组队头元素下边
    int rear;//保存数组队尾元素下一个位置的下边
    /* 循环队列，front和rear到数组尾之后，再移动就循环到数组头*/
}S_LOOPQUEUE;

/*1.创建空队列*/
S_LOOPQUEUE *farsight_creatEmptyQueue()
{
    S_LOOPQUEUE *pstLoopQue = NULL;

    pstLoopQue = (S_LOOPQUEUE*)malloc(sizeof(S_LOOPQUEUE));
    if (NULL == pstLoopQue)
    {
        return NULL;
    }

    /*front和rear的初始化也都是0.直接用memset初始化即可，不许要额外工作*/
    memset(pstLoopQue, 0, sizeof(S_LOOPQUEUE));
    return pstLoopQue;
}
/*2.判断是不是满队列 (rear + 1)%MAXSIZE == front*/
int farsight_isFullLoopQue(S_LOOPQUEUE *pstLoopQue)
{
    if (((pstLoopQue->rear + 1)%MAXSIZE) == pstLoopQue->front)
    {
        return 1;
    }
    
    return 0;
}
/*3.判断是不是空队列 rear == front */
int farsight_isEmptyLoopQue(S_LOOPQUEUE *pstLoopQue)
{
    if (pstLoopQue->rear == pstLoopQue->front)
    {
        return 1;
    }
    return 0;
}

/*4.入队列*/
int farsight_enterLoopQue(S_LOOPQUEUE *pstLoopQue, datatype data)
{
    if (farsight_isFullLoopQue(pstLoopQue))
    {
        printf("queue is full, enter data failed!\n");
        return -1;
    }
    /*先更新数据，然后移动rear*/
    pstLoopQue->buf[pstLoopQue->rear] = data;
    pstLoopQue->rear = (pstLoopQue->rear + 1)%MAXSIZE;
    return 0;
}
/*5.出队列*/
int farsight_delLoopQue(S_LOOPQUEUE *pstLoopQue, int *piData)
{
    if (farsight_isEmptyLoopQue(pstLoopQue))
    {
        printf("queue is empty, del failed!\n");
        return -1;
    }
    *piData = pstLoopQue->buf[pstLoopQue->front];
    pstLoopQue->front = (pstLoopQue->front + 1)%MAXSIZE;
    return 0;
}
/*6.清空队列*/
void farsight_clearLoopQue(S_LOOPQUEUE *pstLoopQue)
{
    free(pstLoopQue);
    return;
}
int main(int argc, const char *argv[])
{
    S_LOOPQUEUE *pstLoopQue = NULL;
    int i = 1;
    int ret = 0;
    int data = 0;

    pstLoopQue = farsight_creatEmptyQueue();
    if (NULL == pstLoopQue)
    {
        printf("create queue failed!\n");
        return 0;
    }
    
    while (1)
    {
        ret = farsight_enterLoopQue(pstLoopQue, i);    
        if (0 != ret)
        {
            break;
        }
        i++;
    }

    while (!farsight_isEmptyLoopQue(pstLoopQue))
    {
        ret = farsight_delLoopQue(pstLoopQue, &data);
        printf("%d ", data);
    }
    printf("\n");
    farsight_clearLoopQue(pstLoopQue);
    
    return 0;
}
