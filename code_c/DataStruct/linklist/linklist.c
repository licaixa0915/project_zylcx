#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int datatype;
typedef struct node
{
    datatype data;         //链表的数据域
    struct node *pNext;    //链表的指针域，指向下一个节点
}S_LINKNODE;

/*****************************************
 *函数名称:farsight_createEmptyLinkList
 *函数作用:创建一个空链表,返回头节点的指针
 *函数入参:void 
 *函数出参:void
 *函数返回值:S_LINKNODE * 头节点指针
 *作者:licaixia
 *创建时间:2017年12月1日
 * ************************************/
S_LINKNODE *farsight_createEmptyLinkList()
{
    S_LINKNODE *pstLinkHead = NULL;

    /*1.为头节点分配空间*/
    pstLinkHead = (S_LINKNODE*)malloc(sizeof(S_LINKNODE));
    if (NULL == pstLinkHead)
    {
        return NULL;
    }
    memset(pstLinkHead, 0, sizeof(S_LINKNODE));
    /*2.初始化头节点的数据域和指针域*/
    pstLinkHead->pNext = NULL;
    /*3.将头节点的指针返回出去 */
    return pstLinkHead;
}

/*打印链表中的所有的元素*/
void farsight_printLinkList(S_LINKNODE *pstLinkHead)
{
    S_LINKNODE *p = pstLinkHead;

    if (NULL == p)
    {
        printf("this list is empty!\n");
        return;
    }

    printf("there is %d num in list\n", p->data);
    while(NULL != p->pNext)
    {
        printf("%d ", p->pNext->data);
        p = p->pNext;
    }
    printf("\n");
    return;
}

/*头插法插入节点*/
int farsight_addNodeToHead(S_LINKNODE *pstLinkHead, datatype data)
{
    /*思考:之前顺序表添加元素的时候，判断表是不是满了，链表有没有满的时候？
     *答案：当然没有了，如果添加元素失败了，那肯定是申请内存就失败了，那时候，就不是添加元素失败的问题了，那就要宕机了*/
    S_LINKNODE *pstLinkNode = NULL;

    /* 1.为头节点分配空间 */
    pstLinkNode = (S_LINKNODE*)malloc(sizeof(S_LINKNODE));
    if (NULL == pstLinkNode)
    {
        printf("malloc failed~\n");
        return -1;
    }
    /* 2.将data填充到头节点中*/
    pstLinkNode->data = data;
    /* 3.添加节点到头节点后边
     *    3.1 将新节点的指针域指向之前头节点之后的那个元素
     *    3.2 将头节点的指针域指向新节点*/
    pstLinkNode->pNext = pstLinkHead->pNext;
    pstLinkHead->pNext = pstLinkNode;
    pstLinkHead->data++;
    return 0;
}
/*尾插法插入节点*/
int farsight_addNodeToTail(S_LINKNODE *pstLinkHead, datatype data)
{
    S_LINKNODE *pstLinkNode = NULL;
    S_LINKNODE *p = NULL;

    /*(1) 为头节点分配空间*/
    pstLinkNode = (S_LINKNODE*)malloc(sizeof(S_LINKNODE));
    if (NULL == pstLinkNode)
    {
        return -1;
    }
    memset(pstLinkNode, 0, sizeof(S_LINKNODE));

    /*(2) 填充data域*/
    pstLinkNode->data = data;
    
    /*(3) 遍历链表到链表尾巴，将节点插入到尾节点后
     *    3.1 尾节点的指针域指向新节点
     *    3.2 新节点指针域置NULL
     * */
    p = pstLinkHead;
    while(NULL != p->pNext)
    {
        p = p->pNext;
    }

    p->pNext = pstLinkNode;
    pstLinkNode->pNext = NULL;
    pstLinkHead->data++;

    return 0;
}
/*删除某个元素 */
int farsight_delNode(S_LINKNODE *pstLinkHead, datatype data)
{
    S_LINKNODE *p = pstLinkHead;
    S_LINKNODE *pstDel = NULL;

    while(NULL != p->pNext)
    {
        if (p->pNext->data == data)
        {
            break;
        }
        p = p->pNext;
    }

    if (NULL == p->pNext)
    {
        return -1;
    }

    pstDel = p->pNext;
    p->pNext = pstDel->pNext;

    free(pstDel);
    pstLinkHead->data--;

    return 0;
}
/*对链表进行排序 */
void farsight_sort(S_LINKNODE *pstLinkHead)
{
    S_LINKNODE *p = NULL;

    datatype tmp = 0;
    int i,j;

    for (i = 0; i < pstLinkHead->data - 1; i++)
    {
        p = pstLinkHead->pNext;

        for (j = 0; j < pstLinkHead->data - 1 - i; j++)
        {
            if (p->data > p->pNext->data)
            {
                tmp = p->data;
                p->data = p->pNext->data;
                p->pNext->data = tmp;

            }
            p = p->pNext;
        }

    }
    
    return;
}

/*插入新节点到有序表中*/
int farsight_insertNodeToOderedList(S_LINKNODE *pstLinkHead, datatype data)
{
    S_LINKNODE *pstLinkNode = NULL;
    S_LINKNODE *p = pstLinkHead;

    /*(1)  为新结点分配空间，用temp保存首地址*/
    pstLinkNode = (S_LINKNODE*)malloc(sizeof(S_LINKNODE));
    if (NULL == pstLinkNode)
    {
        return -1;
    }
    memset(pstLinkNode, 0, sizeof(S_LINKNODE));

    /*(2)  将插入数据存放数据新结点数据域*/
    pstLinkNode->data = data;
    
    /* (3)  循环找到插入位置前一个节点,p。*/
    /* 注：p指向当前节点，单我们比较是后一个节点的数据p->next-data*/
    while (NULL != p->pNext)
    {
        if (p->pNext->data > data)
        {
            break;
        }
        p = p->pNext;
    }
    /*(4)  在p节点的后面插入数据。*/
    /*<1>先让新节点的指针域保存p节点的指针域*/
    /*<2>p的指针域名保存新结点的首地址。*/
    pstLinkNode->pNext = p->pNext;
    p->pNext = pstLinkNode;
    pstLinkHead->data++;
    return 0;
}
/*清空链表*/
void farsight_clearLinkList(S_LINKNODE **pstLinkHead)
{
    S_LINKNODE *p = *pstLinkHead;
    S_LINKNODE *q = NULL;

    while(NULL != p)
    {
        q = p;
        p = p->pNext;
        free(q);
    }
    *pstLinkHead = NULL;
    return;

}

/*将表中的所有元素逆置*/
void farsight_reverseLinkList(S_LINKNODE* pstLinkHead)
{
    /*(1) 定义指针变量p，保存第一个有效数据结点的地址。*/
    S_LINKNODE *p = pstLinkHead->pNext;
    S_LINKNODE *q = NULL;
    
    /*(2) 并把头节点有效数据结点的指针域置为NULL。*/
    pstLinkHead->pNext = NULL;
    while (NULL != p)
    {
        /*(3) 从p开始遍历链表，用q保存p，并插入到头结点后。*/
        q = p;
        p = p->pNext;
        q->pNext = pstLinkHead->pNext;
        pstLinkHead->pNext = q;
    }
    return;
}
/* 两个有序链表进行合并 */
void farsight_mergeTwoLinkList(S_LINKNODE *pstLinkListA, S_LINKNODE*pstLinkListB)
{
    S_LINKNODE *p = pstLinkListA->pNext;
    S_LINKNODE *pPre = pstLinkListA;
    S_LINKNODE *q = pstLinkListB->pNext;

    while (NULL != p && NULL != q)
    {
        if (p->data >= q->data)
        {
            pPre->pNext = q;
            q = q->pNext;
            pPre->pNext->pNext = p;
        }
        pPre = p;
        p = p->pNext;
    }

    if (NULL == p && NULL != q)
    {
        p =q;    
    }

    return;
}
/*判断链表中是否有环*/
int farsight_judgeRingInLinkList(S_LINKNODE*pstLinkHead)
{
    S_LINKNODE *pstSlow = pstLinkHead;
    S_LINKNODE *pstFast = pstLinkHead;


    while (pstSlow != NULL && pstFast->pNext != NULL)
    {
        pstSlow = pstSlow->pNext;
        pstFast = pstFast->pNext->pNext;
        if (pstSlow == pstFast)
        {
            return 1;
        }
    }

    return 0;
}
int main(int argc, const char *argv[])
{

    /*
     * S_LINKNODE *pstLinkHead = NULL;
    S_LINKNODE *pstLinkHeadB = NULL;

    pstLinkHead = farsight_createEmptyLinkList();
    if (NULL == pstLinkHead)
    {
        printf("create linklist failed!\n");
        return 0;
    }
    farsight_addNodeToHead(pstLinkHead, 1);
    farsight_addNodeToHead(pstLinkHead, 5);
    farsight_addNodeToHead(pstLinkHead, 2);
    farsight_addNodeToHead(pstLinkHead, 6);
    farsight_addNodeToHead(pstLinkHead, 8);
    farsight_addNodeToHead(pstLinkHead, 3);
    farsight_addNodeToHead(pstLinkHead, 10);
    farsight_addNodeToTail(pstLinkHead, 20);
    farsight_addNodeToTail(pstLinkHead, 16);
    farsight_addNodeToTail(pstLinkHead, 25);
    farsight_printLinkList(pstLinkHead);
    farsight_delNode(pstLinkHead, 3);
    farsight_printLinkList(pstLinkHead);
    farsight_sort(pstLinkHead);
    farsight_insertNodeToOderedList(pstLinkHead, 22);
    farsight_insertNodeToOderedList(pstLinkHead, 26);
    farsight_insertNodeToOderedList(pstLinkHead, 0);
    //farsight_clearLinkList(&pstLinkHead);
    farsight_printLinkList(pstLinkHead);
    //farsight_reverseLinkList(pstLinkHead);
    farsight_printLinkList(pstLinkHead);

    
    pstLinkHeadB = farsight_createEmptyLinkList();
    if (NULL == pstLinkHeadB)
    {
        printf("create linklist failed!\n");
        return 0;
    }

    farsight_addNodeToTail(pstLinkHeadB, 3);
    farsight_addNodeToTail(pstLinkHeadB, 7);
    farsight_addNodeToTail(pstLinkHeadB, 15);
    farsight_addNodeToTail(pstLinkHeadB, 16);
    farsight_sort(pstLinkHeadB);
    farsight_printLinkList(pstLinkHeadB);
    farsight_mergeTwoLinkList(pstLinkHead, pstLinkHeadB);
    farsight_printLinkList(pstLinkHead);
    pstLinkHeadB->pNext->pNext->pNext = pstLinkHeadB->pNext;
    printf("pstLinkHead %d\n", farsight_judgeRingInLinkList(pstLinkHeadB));
    */

    int N;
    int i;
    int tmp = 0;
    S_LINKNODE *pstHead = NULL;

    pstHead = farsight_createEmptyLinkList();
    if (NULL == pstHead)
    {
        printf("create failed!\n");
        return 0;
    }
    
    scanf("%d", &N);
    for (i = 0; i < N; i++)
    {
        scanf("%d", &tmp);
        farsight_insertNodeToOderedList(pstHead, tmp);
    }

    farsight_printLinkList(pstHead);
    farsight_clearLinkList(&pstHead);
    return 0;
}
