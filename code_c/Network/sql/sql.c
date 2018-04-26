#include <stdio.h>
#include <sqlite3.h>
int n = 1;
typedef int (*sqlite3_callback)(void *arg, int f_num, char **f_value, char **f_name);
int func_callback(void * arg, int f_num, char **f_value, char **f_name)
{
    int i = 0;

    if (n == 1)
    {
        for (i = 0; i < f_num; i++)
        {
            printf("%s\t", f_name[i]);
        }
        printf("\n");
    }
    for (i = 0; i < f_num; i++)
    {
        printf("%s\t", f_value[i]);
    }
    
    printf("\n");
    n++;
    return SQLITE_OK;
}
int main(int argc, const char *argv[])
{
    int ret = 0;
    struct sqlite3 *psqldb = NULL; 
    char **result = NULL;
    int rownum = 0;
    int colnum = 0;
    char *perrmsg = NULL;

    if (argc < 2)
    {
        fprintf(stderr, "Useage: %s database", argv[0]);
        return 0;
    }
    
    ret = sqlite3_open(argv[1], &psqldb);
    if (ret != SQLITE_OK)
    {
        fprintf(stderr, "open failed:%s\n", sqlite3_errmsg(psqldb));
        return 0;
    }
  
#if 0
    //使用sqlite3_get_table实现对sql语句的执行
    ret = sqlite3_get_table(psqldb, "select * from students;", &result, &rownum, &colnum, &perrmsg);
    if (ret < 0)
    {
        fprintf(stderr, "sqlite3_get_table failed:%s\n", sqlite3_errmsg(psqldb));
        return 0;
    }

    //打印头部信息
    int i,j = 0;
    for (i = 0; i < colnum; i++)
    {
        printf("%s\t", result[i]);
    }
    printf("\n");
    int loc = i;
    for (i = 0; i < rownum; i++)
    {
        for (j = 0; j < colnum; j++)
        {
            printf("%s\t", result[loc++]);
        }
        printf("\n");
    }
    //释放结果的表格
    sqlite3_free_table(result);
#endif
#if 1
    //使用sqlite3_exec函数来实现堆数据库的操作
    ret = sqlite3_exec(psqldb, "select * from students;", func_callback, NULL, &perrmsg);
    if (ret < 0)
    {
        fprintf(stderr, "sqlite3_exec:%s", perrmsg);
        return 0;
    }
#endif
    sqlite3_close(psqldb);
    return 0;
}
