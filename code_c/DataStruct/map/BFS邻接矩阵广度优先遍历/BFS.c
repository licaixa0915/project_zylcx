#include <stdio.h>
#include "api.h"

#define MAX 10
typedef struct grapht
{
    int v[MAX];
    int matrix[MAX][MAX];
    int visit[MAX];
}S_GRAPHT;

/*1.打印邻接矩阵*/
void printGrap(S_GRAPHT *pstGrap, int n)
{
    int i,j;
    printf("    ");
    for (i = 0; i < n; i++)
    {
        printf("V%d  ", i); 
    }
    
    for (i = 0; i < n; i++)
    {
        printf("  \nV%d  ", i);
        for (j = 0; j < n; j++)
        {
            printf(" %d  ",pstGrap->matrix[i][j]);
        }
    }
    printf("\n");
    return;
}

/*2.深度优先遍历*/
void BFS(S_GRAPHT *pstGrap, int start)
{
    int i;
    S_LINK_QUEQUE *pstLinkQueue = NULL;
    int data = 0;

    pstLinkQueue = farsight_createEmptyLinkQueue();
    if (NULL == pstLinkQueue)
    {
        return;
    }

    //打印当前图的顶点
    printf("V%d ", start);
    /*将刚刚访问的这个顶点标记成已经访问过*/
    pstGrap->visit[start] = 1;
    /*将其入队列*/
    farsight_enterLinkQueue(pstLinkQueue, start);

    while (!farsight_isEmpty(pstLinkQueue))
    {
        /*将队列头的元素出队列*/
        data = farsight_delLinkQueue(pstLinkQueue);
        /*访问它所有邻接点，并且逐个入队列*/
        for (i = 0; i < MAX; i++)
        {
            if (0 == pstGrap->matrix[data][i])//不与start相邻的则继续
            {
                continue;
            }

            if (pstGrap->visit[i])//已经被访问过了的，继续
            {
                continue;
            }

            /*走到这里，就是与出队列的数据相邻
             * 并且没有被方问过的了*/
            printf("V%d ", i);
            pstGrap->visit[i] = 1;
            farsight_enterLinkQueue(pstLinkQueue, i);
        }
    }
    return;
}
int main(int argc, const char *argv[])
{
    int i,j;
    S_GRAPHT stGrap = {0};
    int n = 0;

    printf("Please input (V0,V1) (V1,V2)......:\n");
    while (scanf("(V%d,V%d)", &i, &j) == 2)
    {
        getchar();
        if (i > n) n = i;
        if (j > n) n = j;
        stGrap.v[i] = 1;
        stGrap.v[j] = 1;
        stGrap.matrix[i][j] = 1;
        stGrap.matrix[j][i] = 1;
    }
    /*清空缓冲区*/
    setbuf(stdin, NULL);
    printGrap(&stGrap, n + 1);
    int s = 0;
    printf("Please input num search from:");
    scanf("%d",&s);
    BFS(&stGrap, s);
    printf("\n");
    return 0;
}
