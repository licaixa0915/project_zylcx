#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHNUM 26
typedef struct chinfo
{
    int count;//保存个数
    char code[256];//保存huffman编码	
}S_CH_INFO;

/*保存26个大写字母在报文中的个数，保存的方法就是A对应数组下标0，B对应数组下标1，也就是ch对应数组下边ch-'A'*/
//int g_szChCount[CHNUM] = {0};
S_CH_INFO g_szChInfo[CHNUM] = {0};
typedef struct huffmannode 
{
    char ch;//叶子节点保存具体的大写字母，其他虚节点保存特殊字符，我们这里就是0就可以
    int weight;//保存当前节点的权重信息，叶子节点是字母出现的次数
    struct huffmannode *pstLChild;//保存左孩子节点
    struct huffmannode *pstRChild;//保存右孩子节点
}S_HUFFMAN_NODE;
S_HUFFMAN_NODE *g_forest[CHNUM] = {0};

/*1.解析用户的输入，将大写字母在用户输入中的次数填充到g_szChInfo中*/
void farsight_statisticStr(char *str)
{
    char *p = str;

    while (*p)
    {
        if (*p <= 'Z' && *p >= 'A')
        {
            //g_szChCount[*p - 'A']++;
            g_szChInfo[*p - 'A'].count++;
        }
        p++;
    }
    return;
}

/*2.将g_szChInfo中的数据打印出来*/
void farsight_printCount()
{
    int i;

    for (i = 0; i < CHNUM; i++)
    {
        /*打印所有非0项*/
        if (g_szChInfo[i].count)
        {
        		printf("%c:%d(%s)  ", i + 'A', g_szChInfo[i].count, g_szChInfo[i].code);
        }
            
    }
    printf("\n");
    return;
}
/*3.创建二叉树的节点，返回节点指针*/
S_HUFFMAN_NODE *farsight_createHuffmanNode()
{
    S_HUFFMAN_NODE *pstNode = NULL;

    pstNode = (S_HUFFMAN_NODE*)malloc(sizeof(S_HUFFMAN_NODE));
    memset(pstNode, 0, sizeof(S_HUFFMAN_NODE));

    return pstNode;
}

/*4.遍历森林，找到其中两个权重低的节点*/
void farsight_getMinNodes(S_HUFFMAN_NODE **pstForest,S_HUFFMAN_NODE **ppstMin, S_HUFFMAN_NODE** ppstMin1)
{
    int i;
    int minpos = 0;
    int min = 0xffff;

    for (i = 0; i < CHNUM; i++)
    {
        if ((pstForest[i]) && (pstForest[i]->weight < min))
        {
            *ppstMin = pstForest[i];
            minpos = i;
            min = pstForest[i]->weight;
        }
    }
    
    /*将两个树从森林中删除*/
    pstForest[minpos] = 0;
    min = 0xffff;
    for (i = 0; i < CHNUM; i++)
    {
        if ((pstForest[i]) && (pstForest[i]->weight < min))
        {
            *ppstMin1 = pstForest[i];
            minpos = i;
            min = pstForest[i]->weight;
        }
    }
    /*将两个树从森林中删除*/
    pstForest[minpos] = 0;
    return;
}
/*5.根据统计结果，创建huffman树*/
S_HUFFMAN_NODE* farsight_createHuffmanTree()
{
    S_HUFFMAN_NODE *szForest[CHNUM] = {0};//保存森林信息
    S_HUFFMAN_NODE *pstNode = NULL;
    S_HUFFMAN_NODE *pstMin = NULL;
    S_HUFFMAN_NODE *pstMin1 = NULL;
    int treenum = 0;//保存森林中树的个数
    int i;
    /*1.初始化森林，将所有的报文中存在的字母，建立节点，加入到森林中，因为最多26个字母，所以森林中树的个数
     * 最多就26个，所以szfrest的元素个数就26个就够了*/
    for (i = 0; i < CHNUM; i++)
    {
        if (g_szChInfo[i].count)
        {
            pstNode = farsight_createHuffmanNode();
            if (NULL == pstNode)
            {
                printf("create node error!\n");
                return;
            }

            pstNode->ch = i+'A';
            pstNode->weight = g_szChInfo[i].count;
            szForest[treenum] = pstNode;
            treenum++;
        }
    }

    /*2.遍历森林，找到其中两个权重最小的节点，保存，并将其在森林中删除*/
    while (1 != treenum)
    {
        /*找到森林中两个权重最小的节点*/
        farsight_getMinNodes(szForest, &pstMin, &pstMin1);
        treenum -= 2;
        pstNode = farsight_createHuffmanNode();
        if (NULL == pstNode)
        {
            printf("create error!\n");
            return;
        }
        pstNode->weight = pstMin->weight + pstMin1->weight;
        /*组成一颗新树*/
        pstNode->pstLChild = pstMin;
        pstNode->pstRChild = pstMin1;
        /*将新树加入到森林中*/
        for (i = 0; i < CHNUM; i++)
        {
            if (NULL == szForest[i])
            {
                szForest[i] = pstNode;
                break;
            }
        }
        treenum++;
    }
    return pstNode;
}
/*6.中序遍历huffman树,返回得到的编码字符串*/
void farsight_printHuffInOrder(S_HUFFMAN_NODE *pstHuffmanTree, char *code, char*pCode)
{
    static int i = 0;


    if (NULL == pstHuffmanTree)
    {
        return;
    }

    if (pstHuffmanTree->ch)//叶子节点
    {
        /*将叶子节点对应的编码保存下来*/
        code[i] = '\0';
        memcpy(g_szChInfo[pstHuffmanTree->ch-'A'].code, code, strlen(code));
        printf("%c(%s) ", pstHuffmanTree->ch, code);
        pCode = strcat(pCode, code);
    }

    if (pstHuffmanTree->pstLChild)
    {
        code[i] = '0';
        i++;
    }
    farsight_printHuffInOrder(pstHuffmanTree->pstLChild, code, pCode);
    

    if (pstHuffmanTree->pstRChild)
    {
        code[i] = '1';
        i++;
    }
    farsight_printHuffInOrder(pstHuffmanTree->pstRChild, code, pCode);
    i--;
    return;
}

void farsight_printStrCode(char *str)
{
    char *p = str;

    while(*p)
    {
        printf("%s", g_szChInfo[*p-'A'].code);
        p++;
    }
    printf("\n");
    return;
}
/*huffman解码函数，根据传入的编码的内容，解出字符串*/
void farsight_decodeingHuff(char *str, S_HUFFMAN_NODE *pstHuffmanTree)
{
    char *p = str;
    char decStr[1024] = {0};
    int num = 0;
    S_HUFFMAN_NODE *pstNode = pstHuffmanTree;

    while (*p)
    {
        if ((*p)-'0')//1
        {
            pstNode = pstNode->pstRChild;
        }
        else//0
        {
            pstNode = pstNode->pstLChild;
        }
        if (pstNode && pstNode->ch)//叶子节点
        {
            decStr[num] = pstNode->ch;
            num++;
            pstNode = pstHuffmanTree;
        }
        p++;
    }

    printf("%s\n", decStr);

    return;
}

int main(int argc, const char *argv[])
{
    char str[1024] = {0};
    char code[256] = {0};
    char getCode[1024] = {0};
    S_HUFFMAN_NODE *pstHuffmanTree = NULL;

    /*接收用户的一串输入*/
    printf("please input a string:\n");
    str[1023] = '\0';
    gets(str);

    farsight_statisticStr(str);
    
    pstHuffmanTree = farsight_createHuffmanTree();
    if (NULL == pstHuffmanTree)
    {
        printf("create error!\n");
        return 0;
    }
    /*中序遍历huffman树看一下建树是否正确*/
    farsight_printHuffInOrder(pstHuffmanTree, code, getCode);
    //farsight_printCount();
    printf("\n");
    farsight_printStrCode(str);
    printf("%s\n", getCode);
    //farsight_decodeingHuff(getCode, pstHuffmanTree);
    //farsight_decodeingHuff("0111011000111010110011", pstHuffmanTree);
    return 0;
}
