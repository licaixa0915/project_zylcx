#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char datatype;
typedef struct bitreenode
{
    datatype data;//节点数据
    struct bitreenode *pstLChild;//左孩子节点指针
    struct bitreenode *pstRChild;//右孩子节点指针
}S_BITREE_NODE;

/*1.创建完全二叉树节点，初始化，返回节点指针*/
S_BITREE_NODE* farsight_createBiTreeNode(datatype data)
{
    S_BITREE_NODE *pstNode = NULL;

    /*为树的节点在堆上分配空间*/
    pstNode = (S_BITREE_NODE*)malloc(sizeof(S_BITREE_NODE));
    if (NULL == pstNode)
    {
        return NULL;
    }
    memset(pstNode, 0, sizeof(S_BITREE_NODE));

    /*填充数据，初始化相关指针*/
    pstNode->data = data;
    pstNode->pstLChild = NULL;
    pstNode->pstRChild = NULL;

    return pstNode;
}

/*2.创建一个完全二叉数树,返回的是跟节点的指针*/
S_BITREE_NODE *farsight_createBiTree(char *pcData, int n, int num)
{
    /*参数:pcData需要放到树中的数据指针  
     *     n     需要放到树中的数据的个数
     *     m     当前节点的编号，根节点从1开始编号*/
    S_BITREE_NODE *pstRoot = NULL;

    pstRoot = farsight_createBiTreeNode(pcData[num-1]);
    if (NULL == pstRoot)
    {
        printf("create failed!\n");
        return NULL;
    }

    /*判断当前编号的节点是不是有左孩子，没有则创建*/
    if ((2 * num) <= n)
    {
        pstRoot->pstLChild = farsight_createBiTree(pcData, n, num*2);
    }

    /*判断当前编号的节点是不是有右孩子，没有则创建*/
    if ((2 * num + 1) <= n)
    {
        pstRoot->pstRChild = farsight_createBiTree(pcData, n, num*2+1);
    }
    return pstRoot;
}

/*3.前序遍历*/
void farsight_preOder(S_BITREE_NODE *pstRoot)
{
    if (NULL == pstRoot)
    {
        return;
    }

    printf("%c ", pstRoot->data);
    farsight_preOder(pstRoot->pstLChild);
    farsight_preOder(pstRoot->pstRChild);
    return;
}
/*4.中序遍历*/
void farsight_inOder(S_BITREE_NODE *pstRoot)
{
    if (NULL == pstRoot)
    {
        return;
    }

    farsight_inOder(pstRoot->pstLChild);
    printf("%c ", pstRoot->data);
    farsight_inOder(pstRoot->pstRChild);
    return;
}
/*5.后序遍历*/
void farsight_postOder(S_BITREE_NODE *pstRoot)
{
    if (NULL == pstRoot)
    {
        return;
    }

    farsight_postOder(pstRoot->pstLChild);
    farsight_postOder(pstRoot->pstRChild);
    printf("%c ", pstRoot->data);
    return;
}
int main(int argc, const char *argv[])
{
    S_BITREE_NODE *pstRoot = NULL;
    char szData[10] = {'A','B','C','D','E','F','G','H','I','J'};

    pstRoot = farsight_createBiTree(szData, 10, 1);
    if (NULL == pstRoot)
    {
        printf("create failed!\n");
        return 0;
    }

    printf("printf by pre-oder:");
    farsight_preOder(pstRoot);
    printf("\nprintf by in-oder:");
    farsight_inOder(pstRoot);
    printf("\nprintf by post-oder:");
    farsight_postOder(pstRoot);
    printf("\n");
    return 0;
}
