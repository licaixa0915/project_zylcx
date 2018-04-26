#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*一：数据类型*/
/*1.栈节点数据信息*/
typedef int datatype;
typedef struct node
{
    datatype data;//节点的元素类型
    struct node *pNext;
}S_STACKNODE;

/*2.栈的头信息*/
typedef struct STACK
{
    S_STACKNODE *top;//保存栈顶信息，也就是链表的头节点
    int n;   //保存一下栈的大小
}S_LINKSTACK;

/*二：基本操作*/
/*1.创建空的链栈*/
S_LINKSTACK *farsight_createEmptyLinkStack()
{
    S_LINKSTACK *pstLinkStack = NULL;

    /*1.为栈头信息在堆区分配空间*/
    pstLinkStack = (S_LINKSTACK*)malloc(sizeof(S_LINKSTACK));
    if (NULL == pstLinkStack)
    {
        return NULL;
    }

    /*2.初始化栈头*/
    memset(pstLinkStack, 0, sizeof(S_LINKSTACK));
    /*3.将栈头返回出去*/
    return pstLinkStack;
}

/*2.判断栈是不是空栈*/
int farsight_isEmptyLinkStack(S_LINKSTACK *pstLinkStack)
{
    return (NULL == pstLinkStack->top) ? 1:0;
}

/*3.入栈（类似于链表的头插法）*/
int farsight_pushLinkStack(S_LINKSTACK *pstLinkStack, datatype data)
{
    /*1.为（链表）新结点在堆区分配空间，用temp保存 */
    S_STACKNODE *pstNewNode = (S_STACKNODE *)malloc(sizeof(S_STACKNODE));
    if(NULL == pstNewNode)
    {
        printf("malloc new data is fail\n");    
        return -1;
    }
                
    
    memset(pstNewNode,0,sizeof(S_STACKNODE));
    /*2.把数据data存储到pstNewNOde中*/
    pstNewNode->data = data;
    /*3.通过栈顶指针插入链表中*/
    pstNewNode->pNext = pstLinkStack->top;    
    pstLinkStack->top = pstNewNode; 
    /*4.更新s中栈中数据n的值。pstLinkStack->n++;*/
    pstLinkStack->n ++;
    return 0;
}

/*4.出栈*/
int farsight_popLinkStack(S_LINKSTACK *pstLinkStack)
{
    datatype data = 0;
    S_STACKNODE *p = NULL;

    /*1.保存栈顶数据*/
    data = pstLinkStack->top->data;
    /*2.保存栈顶指针*/
    p = pstLinkStack->top;
    /*3.更新栈顶指针信息*/
    pstLinkStack->top = pstLinkStack->top->pNext;
    /*4.释放前栈顶指针*/
    free(p);

    /*5.维护n*/
    pstLinkStack->n--;

    return data;
}
int main(int argc, const char *argv[])
{
    S_LINKSTACK *pstLinkStack = NULL;
    datatype data = 0;

    pstLinkStack = farsight_createEmptyLinkStack();
    if (NULL == pstLinkStack)
    {
        printf("create failed\n");
        return 0;
    }

    farsight_pushLinkStack(pstLinkStack, 2);
    farsight_pushLinkStack(pstLinkStack, 8);
    farsight_pushLinkStack(pstLinkStack, 6);
    farsight_pushLinkStack(pstLinkStack, 20);
    farsight_pushLinkStack(pstLinkStack, 1);
    farsight_pushLinkStack(pstLinkStack, 10);
    while (!farsight_isEmptyLinkStack(pstLinkStack))
    {
        data = farsight_popLinkStack(pstLinkStack);
        printf("%d ", data);
    }
    printf("\n");

    free(pstLinkStack);
    return 0;
}
