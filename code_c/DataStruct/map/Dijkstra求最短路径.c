#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 6
#define M 0XFFFFFF
typedef struct grapht
{
    int v[MAX];
    int matrix[MAX][MAX];
    int visit[MAX];
    int matrixForDijks[MAX][MAX];
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
void Dijkstra(S_GRAPHT *pstGrap, int start, int n)
{
    int s[MAX] = {0};//s集
    int dist[MAX] = {0};
    char path[MAX][2*MAX] = {0};
    char buf[2*MAX] = {0};
    int i = 0;
    int count = 0;
    int min = M;
    int min_v = 0;

    /*1.初始化dist,和path*/
    for (i = 0; i < MAX; i++)
    {
        dist[i] = pstGrap->matrixForDijks[start][i];
        sprintf(path[i],"%s%d", "V", start);
    }

    /*2.把开始的start顶点加入到s集合中*/
    s[start] = 1;

    while (count < n - 1)
    {
        min = M;
        /*3.找到dist中，最短的路径及对应的顶点*/
        for (i = 0; i < MAX; i++)
        {
            if ((!s[i]) && (dist[i] < min))
            {
                min = dist[i];
                min_v = i;
            }
        }
        /*4.将min_v加入到s集合中*/
        s[min_v] = 1;
        /*5.更新path*/
        memset(buf,0,MAX);
        sprintf(buf, "->%s%d", "V", min_v);
        strcat(path[min_v],buf);

        /*6.更新dist*/
        for (i = 0; i < MAX; i++)
        {
            if (pstGrap->matrix[min_v][i] && (!s[i]))
            {
                if (pstGrap->matrixForDijks[min_v][i] + dist[min_v] < dist[i])
                {
                    dist[i] = pstGrap->matrixForDijks[i][min_v] + dist[min_v];
                    strcpy(path[i], path[min_v]);
                }
            }
        }
        count++;
    }

    for (i = 0; i < n; i++)
    {
        printf("V%d->V%d:%d  %s\n", start,i,dist[i], path[i]);
    }
    return;
}

int main(int argc, const char *argv[])
{
    int i,j,p;
    int l,m,n;
    S_GRAPHT stGrap = {0};
    
    /*1.为Dijkstra算法初始化matrixForDijks*/
    for (l = 0; l < MAX; l++)
    {
        for (m = 0; m < MAX; m++)
        {
            if (l == m)
            {
                stGrap.matrixForDijks[l][m] = 0;
            }
            else
            {
                stGrap.matrixForDijks[l][m] = M;
            }
        }
    }

    printf("Please input (V0,V1,20) (V1,V2,10)......:\n");
    while (scanf("(V%d,V%d,%d)", &i, &j, &p) == 3)
    {
        getchar();
        if (i > n) n = i;
        if (j > n) n = j;
        stGrap.v[i] = 1;
        stGrap.v[j] = 1;
        stGrap.matrix[i][j] = 1;
        stGrap.matrix[j][i] = 1;
        stGrap.matrixForDijks[i][j] = p;
        stGrap.matrixForDijks[j][i] = p;
    }
    /*清空缓冲区*/
    setbuf(stdin, NULL);
    printGrap(&stGrap, n + 1);
    int s = 0;
    printf("Please input num search from:");
    scanf("%d",&s);
    DFS(&stGrap, s);
    printf("\n");
    Dijkstra(&stGrap, 0, n+1);
    return 0;
}
