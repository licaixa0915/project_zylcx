#include <stdio.h>
#define MAX 6
#define M 99
typedef struct grapht
{
    int v[MAX];
    int matrix[MAX][MAX];
    int visit[MAX];
    int e[MAX][MAX];//Floyd算法使用的矩阵
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
void DFS(S_GRAPHT *pstGrap, int start)
{
    int i;

    //打印当前图的顶点
    printf("V%d ", start);
    /*将刚刚访问的这个顶点标记成已经访问过*/
    pstGrap->visit[start] = 1;

    /*接着访问这个顶点的下一个没有被访问过的邻接顶点*/
    for (i = 0; i < MAX; i++)
    {
        if (0 == pstGrap->matrix[start][i])//不与start相邻的则继续
        {
            continue;
        }

        if (pstGrap->visit[i])//已经被访问过了的，继续
        {
            continue;
        }

        /*走到这里，肯定是与start相邻并且没有被访问过，则访问它*/
        DFS(pstGrap, i);
    }
    return;
}
void printfE(S_GRAPHT *pstGrap, int n);
void floyd(S_GRAPHT *pstGrap, int start, int n)
{
    int i,j,k;

    printfE(pstGrap,n);

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            for (k = 0; k < n; k++)
            {
                if (pstGrap->e[j][k] > pstGrap->e[j][i] + pstGrap->e[i][k])
                {
                    pstGrap->e[j][k] = pstGrap->e[j][i] + pstGrap->e[i][k];
                    pstGrap->e[k][j] = pstGrap->e[j][k];
                }
            }
        }
        //printfE(pstGrap,n);
        //printf("\n");
    }

    for (i = 0; i < n; i++)
    {
        printf("V%d->V%d:%d\n", start,i, pstGrap->e[start][i]);    
    }
    
    return;
}
void printfE(S_GRAPHT *pstGrap,int n)
{
    int i,j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%2d  ", pstGrap->e[i][j]);
        }
        printf("\n");
    }
    return;
}
int main(int argc, const char *argv[])
{
    int i,j;
    S_GRAPHT stGrap = {0};
    int n = 0;
    int p = 0;

    for (i = 0; i < MAX; i++)
    {
        for (j = 0; j < MAX; j++)
        {
            if (i == j) 
            {
                stGrap.e[i][j] = 0;
            }
            else
            {
                stGrap.e[i][j] = M;
                stGrap.e[j][i] = M;
            }
        }
    }
    printf("Please input (V0,V1) (V1,V2)......:\n");
    while (scanf("(V%d,V%d,%d)", &i, &j, &p) == 3)
    {
        getchar();
        if (i > n) n = i;
        if (j > n) n = j;
        stGrap.v[i] = 1;
        stGrap.v[j] = 1;
        stGrap.matrix[i][j] = 1;
        stGrap.matrix[j][i] = 1;
        stGrap.e[i][j] = p;
        stGrap.e[j][i] = p;
    }

    /*清空缓冲区*/
    setbuf(stdin, NULL);
    printGrap(&stGrap, n + 1);
    int s = 0;
    printf("Please input num search from:");
    scanf("%d",&s);
    DFS(&stGrap, s);
    printf("\n");
    floyd(&stGrap, 0, n+1);
    return 0;
}
