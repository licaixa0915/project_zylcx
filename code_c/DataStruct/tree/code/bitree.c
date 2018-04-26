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
/*0.非递归创建完全二叉树*/
S_BITREE_NODE * farsight_createBiTree0(int n, char *pcData)
{
    S_BITREE_NODE **ppBitree = NULL;
    S_BITREE_NODE*pstRoot = NULL;
    int i = 0;

    ppBitree = (S_BITREE_NODE**)malloc(sizeof(S_BITREE_NODE*)*(n+1));
    memset(ppBitree, 0, sizeof(S_BITREE_NODE*)*(n+1));
    for (i = 1; i <= n; i++)
    {
        ppBitree[i] = (S_BITREE_NODE*)malloc(sizeof(S_BITREE_NODE));
        memset(ppBitree[i], 0, sizeof(S_BITREE_NODE));
        ppBitree[i]->data = pcData[i-1];
    }

    for (i = n; i >= 2; i--)
    {
        if (i%2)//i%2==1的话，说明这个是右孩子,i/2为其父节点
        {
            ppBitree[i/2]->pstRChild = ppBitree[i];
        }
        else//说明这个是左孩子，i/2为其父节点
        {
            ppBitree[i/2]->pstLChild = ppBitree[i];
        }
    }
    pstRoot = ppBitree[1];
    free(ppBitree);
    return pstRoot;
}
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

/*扩展：前序，中序，后序构造树*/
/*1.前序构造树*/
S_BITREE_NODE * farsight_createTreeByPre(char *szData)
{
    S_BITREE_NODE *pstRoot = NULL;
    static int num = 0;

    if (!(*(szData+num)))
    {
        return NULL;
    }
    /*创建根节点*/
    pstRoot = farsight_createBiTreeNode(*(szData+num));
    num++;
    /*创建左子树*/
    pstRoot->pstLChild = farsight_createTreeByPre(szData);
    /*创建右子树*/
    pstRoot->pstRChild = farsight_createTreeByPre(szData);
    
    return pstRoot;
}
/*2.求树的节点的个数*/
int farsight_getNodeNum(S_BITREE_NODE *pstRoot)
{
    int iLnum = 0;
    int iRnum = 0;

    if (pstRoot)
    {
        if (pstRoot->pstLChild)//左孩子节点存在
        {
            iLnum = farsight_getNodeNum(pstRoot->pstLChild);
        }

        if  (pstRoot->pstRChild)//右孩子节点存在
        {
            iRnum = farsight_getNodeNum(pstRoot->pstRChild);
        }
    }
    return iLnum + iRnum + 1;
}

/*3.求树的高度*/
int farsight_getTreeDeep(S_BITREE_NODE *pstRoot)
{
    int iLDeep = 0;
    int iRDeep = 0;
    if (pstRoot)
    {
        if (pstRoot->pstLChild)
            iLDeep = farsight_getTreeDeep(pstRoot->pstLChild);
        
        if (pstRoot->pstRChild)
            iRDeep = farsight_getTreeDeep(pstRoot->pstRChild);
    }
    return iLDeep > iRDeep ? iLDeep + 1: iRDeep + 1;
}
int main(int argc, const char *argv[])
{
    S_BITREE_NODE *pstRoot = NULL;
    char szData[10] = {'A','B','C','D','E','F','G','H','I','J'};
    char szInData[11] = {'H','D','I','B','J','E','A','F','C','G',0};
    char szPreData[11] = {'A','B','D','H','I','E','J','C','F','G',0};
    
    /*
    pstRoot = farsight_createBiTree(szData, 10, 1);
    if (NULL == pstRoot)
    {
        printf("create failed!\n");
        return 0;
    }*/
    //pstRoot = farsight_createTreeByPre(szPreData);
    pstRoot = farsight_createBiTree0(10,szData);

    printf("the num of tree is %d\n", farsight_getNodeNum(pstRoot));
    printf("the deep of tree is %d\n", farsight_getTreeDeep(pstRoot));
    printf("printf by pre-oder:");
    farsight_preOder(pstRoot);
    printf("\nprintf by in-oder:");
    farsight_inOder(pstRoot);
    printf("\nprintf by post-oder:");
    farsight_postOder(pstRoot);
    printf("\n");
    return 0;
}
