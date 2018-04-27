#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_MAX_SIZE 1024
char * add_quota(char *buf)
{
    char *p = (char *)malloc(LINE_MAX_SIZE*2);
    char *p_bak = p;
    char *q = buf;
    int j = 0;

    if (p == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    memset(p, 0, LINE_MAX_SIZE*2);
    while (*q)
    {
        if (*q == '\'')
        {
            *p = '\'';
            p++;
        }
        
        *p = *q;

        p++;
        q++;
    }
    memcpy(buf, p_bak, strlen(p_bak));
    free(p_bak);
    return buf;
}

void load_word(FILE *fp, struct sqlite3 *pdb)
{
    char *buf = NULL;
    char *word = NULL;
    int i = 0;
    char *explain = NULL;
    int ret = 0;
    char **result = NULL;

    buf = (char *)malloc(LINE_MAX_SIZE*2);
    if (buf == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        return;
    }
    memset(buf, 0, LINE_MAX_SIZE*2);
    //一次读取一行的数据
    while (fgets(buf, LINE_MAX_SIZE, fp))
    {
        i = 0;
        explain = NULL;
        char sql[1024] = {0};

        //去掉一行数据中的最后的\r\n
        buf[strlen(buf) - 1] = '\0';
        if (buf[strlen(buf) - 2] == '\r')
            buf[strlen(buf) - 2] = '\0';
        //处理描述中的',遇到单引号则变成双引号
        word = add_quota(buf);
        //扫描到第一个空格，置为'\0', 并保存后边的指针
        while (*(word+i))
        {
            if (*(word+i) == ' ')
            {
                explain = word + i + 1;
                *(word + i) = '\0';
                break;
            }
            i++;
        }

        //处理explain
        while (*explain == ' ')
        {
            explain++;
        }
        printf("%s\t%s\n", word, explain);
        //拼接sql语句
        sprintf(sql, "insert into word_table values('%s','%s');", word, explain);

        printf("sql:%s\n", sql);
        ret = sqlite3_get_table(pdb, sql, &result, NULL, NULL, NULL);
        if (ret < 0)
        {
            fprintf(stderr, "sqlite3_get_table");
            return;
        }
        memset(buf, 0, LINE_MAX_SIZE*2);

    }

    return;
}

int main(int argc, const char *argv[])
{
    int ret = 0;
    struct sqlite3 *pdb = NULL;
    FILE* fp = NULL;
    char buf[1024] = {0};

    if (argc < 3)
    {
        fprintf(stderr, "Useage:%s txt_filename database", argv[0]);
        return 0;
    }

    //打开数据库
    ret = sqlite3_open(argv[2], &pdb);
    if (ret < 0)
    {
        fprintf(stderr, "sqlite3_open failed:%s\n", sqlite3_errmsg(pdb));
        return 0;
    }

    //打开要导入的txt文件
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        perror("fopen");
        return 0;
    }

    //加载txt文件中的内容到数据库中
    load_word(fp, pdb);

    sqlite3_close(pdb);
    fclose(fp);
    return 0;
}
