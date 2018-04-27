#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int datatype;
typedef struct node
{
    datatype data;
    struct node *pstLChild;
    struct node *pstRChild;
}S_BST_NODE;

S_BST_NODE *searchInsertNode(S_BST_NODE *pstRoot, datatype key)
{
    S_BST_NODE *p = pstRoot;
    while(p)
    {
        if (key > p->data)//比p的data大，遍历右子树继续找
        {
            if (p->pstRChild)
            {
                p = p->pstRChild;
            }
            else
            {
                return p;
            }
        }
        else if (key < p->data)
        {
            if (p->pstLChild)
            {
                p = p->pstLChild;
            }
            else
            {
                return p;
            }
        }
        else
        {
            return NULL;//二叉查找树中不允许有重复的数据
        }
    }
    return NULL;
}
/*1.二叉查找树的创建-非递归实现*/
S_BST_NODE *createBSTTree(datatype *pData, int n)
{
    S_BST_NODE *pstRoot = NULL;
    S_BST_NODE *pstNode = NULL;
    S_BST_NODE *pstFather = NULL;
    int i;

    for (i = 0; i < n; i++)
    {
        pstNode = (S_BST_NODE*)malloc(sizeof(S_BST_NODE));
        if (NULL == pstNode)
        {
            printf("malloc failed!\n");
            return NULL;
        }
        memset(pstNode, 0, sizeof(S_BST_NODE));
        pstNode->data = pData[i];
        if (0 == i)
        {
            pstRoot = pstNode;//数组下标是0的表示根节点
            continue;
        }
        /*找到可以插入的节点，我们封装成函数*/
        pstFather = searchInsertNode(pstRoot, pstNode->data);
        if (NULL == pstFather)
        {
            continue;
        }
        printf("search node data is %d\n", pstFather->data);
        if (pstNode->data > pstFather->data)//放在右边
        {
            pstFather->pstRChild = pstNode;
        }
        else//左边   不会存在相等，因为相等了就return NULL了，上边就拦住了
        {
            pstFather->pstLChild = pstNode;
        }
    }
    
    return pstRoot;
}
/*2.二叉查找树的遍历*/
void printByInOrder(S_BST_NODE *pstRoot)
{
    if (NULL == pstRoot)
        return;

    printByInOrder(pstRoot->pstLChild);
    printf("%d ", pstRoot->data);
    printByInOrder(pstRoot->pstRChild);
    return;
}
/*3.二叉树的查找*/
S_BST_NODE *searchNode(S_BST_NODE *pstRoot, datatype key)
{ 
    S_BST_NODE *p = pstRoot;
    while(p)
    {
        if (key > p->data)//比p的data大，遍历右子树继续找
        {
            if (p->pstRChild)
            {
                p = p->pstRChild;
            }
            else
            {
                return NULL;
            }
        }
        else if (key < p->data)
        {
            if (p->pstLChild)
            {
                p = p->pstLChild;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return p;//二叉查找树中不允许有重复的数据
        }
    }
    return NULL;
}

/*4.二叉查找树的删除*/
S_BST_NODE * delNode(S_BST_NODE *pstRoot, datatype key)
{
    S_BST_NODE *p = pstRoot;
    S_BST_NODE *pfather = NULL;
    int find = 0;
    while(p)
    {
        if (key > p->data)//比p的data大，遍历右子树继续找
        {
            if (p->pstRChild)
            {
                pfather = p;
                p = p->pstRChild;

            }
        }                     
        else if (key < p->data)
        {
            if (p->pstLChild)
            {
                pfather = p;
                p = p->pstLChild;
            }
        }
        else
        {
            find = 1;
            break;
        }
    }

    if (find && pfather)//找到了
    {
        if (p->pstLChild && p->pstRChild)
        {
            /*找要删除节点的右节点的左节点中左孩子为NULL的节点*/
            S_BST_NODE *pTmp = p->pstRChild;
            while (pTmp->pstLChild)
            {
                pTmp = pTmp->pstLChild;
            }
            pTmp->pstLChild = p->pstLChild;
            if (p->data < pfather->data)
            {
                pfather->pstLChild = p->pstRChild;
            }
            else
            {
                pfather->pstRChild = p->pstRChild;
            }
            free(p);
        }
        else
        {
            if (p->data < pfather->data)
            {
                if (p->pstRChild)
                { 
                    pfather->pstLChild = p->pstRChild;
                }
                else if(p->pstLChild)
                {
                    pfather->pstLChild = p->pstLChild;
                }
                else
                {
                    pfather->pstLChild = NULL;
                }
            }
            else
            {
                if (p->pstRChild)
                { 
                    pfather->pstRChild = p->pstRChild;
                }
                else if(p->pstLChild)
                {
                    pfather->pstRChild = p->pstLChild;
                }
                else
                {
                    pfather->pstRChild = NULL;
                }
            }
            free(p);
        }
    }

    return pstRoot;
}   

int main(int argc, const char *argv[])
{
    datatype data[] = {62,88,58,47,35,73,51,99,37,93};
    S_BST_NODE *pstRoot = NULL;
    S_BST_NODE *pstTmp = NULL;
    int len = sizeof(data)/sizeof(data[0]);

    pstRoot = createBSTTree(data, len);
    if (NULL == pstRoot)
    {
        printf("create error!\n");
        return 0;
    }
    pstRoot = delNode(pstRoot, 99);
    printByInOrder(pstRoot);
    printf("\n");
    pstTmp = searchNode(pstRoot, 99);
    if (pstTmp)
    {
        printf("%d\n", pstTmp->data);
    }
    else
    {
        printf("not find!\n");
    }
    return 0;
}
