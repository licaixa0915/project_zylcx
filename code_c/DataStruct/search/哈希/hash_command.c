#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHLEN     26
#define COMMAND_MAXLEN 15
#define END  "end"
typedef struct node
{
    char command[COMMAND_MAXLEN+1];
    struct node *pNext;
}S_HASH_NODE;

/*1.插入数据到hash表中*/
int insertDataToHash(S_HASH_NODE **pszHash, char *key)
{
    int k = (*key)-'a';//构造hash函数，按照首字母来存储
    S_HASH_NODE **ppstNode = &pszHash[k];

    while(*ppstNode)
    {
        ppstNode = &((*ppstNode)->pNext);
    }

    *ppstNode = (S_HASH_NODE*)malloc(sizeof(S_HASH_NODE));
    if (NULL == *ppstNode)
    {
        printf("malloc failed!\n");
        return -1;
    }
    memset(*ppstNode, 0, sizeof(S_HASH_NODE));
    strcpy((*ppstNode)->command, key);
    return 0;
}

/*判断str1是不是以str2开头的*/
int isBeginWith(char*str1, char *str2)
{
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    int i;

    if (len1 < len2 || len1 == 0 || len2 == 0)
    {
        return 0;
    }

    for (i = 0; i < len2; i++)
    {
        if (str1[i] != str2[i])
        {
            return 0;
        }
    }

    return 1;

}
/*2.根据输入的command,输出tab的结果*/
void printTabRes(S_HASH_NODE **pszHash, char *command)
{
    int k = (*command)-'a';
    S_HASH_NODE *pstNode = NULL;

    pstNode = pszHash[k];
    while(pstNode)
    {
        if (isBeginWith(pstNode->command, command))
        {
            printf("%s ", pstNode->command);
        }
        pstNode = pstNode->pNext;
    }
    printf("\n");
    return;
}

int main(int argc, const char *argv[])
{
    S_HASH_NODE *szHash[HASHLEN] = {0}; //hash表
    char command[COMMAND_MAXLEN+1] = {0};

    printf("please input command what you want to add, \"end\" is ended:\n");
    while(1)
    {
        memset(command, 0, COMMAND_MAXLEN);
        scanf("%s", command);
        if (0 == strcmp(command, END))//用户输入了end结束标记，停止输入
        {
            break;
        }
        insertDataToHash(szHash, command);
    }

    printf("PLease input the head of the command:\n");
    memset(command, 0, COMMAND_MAXLEN);
    scanf("%s", command);
    printTabRes(szHash, command);
    return 0;
}
