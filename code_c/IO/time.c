#include <stdio.h>
#include <time.h>

int main(int argc, const char *argv[])
{
    FILE *fp = NULL;
    time_t time_now = 0;
    struct tm*ptm = NULL;
    char buf[512] = {0};
    int line = 0;

    fp = fopen("test.txt", "a+");
    if (NULL == fp)
    {
        perror("fopen");
        return 0;
    }
    // 统计fp对应的文件中一共有多少行
    // 使用fgets来获得。fgets是遇到\n就读取一次
    while(fgets(buf, sizeof(buf), fp))
    {
        line++;
    }

    while(1)
    {
        time_now = time(NULL);
        ptm = localtime(&time_now);
        if (NULL == ptm)
        {
            perror("localtime");
            return 0;
        }
        printf("%d, %4d-%d-%d %2d:%2d:%02d\n", ++line, ptm->tm_year + 1900, ptm->tm_mon + 1,
                ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
        fprintf(fp, "%d, %4d-%d-%d %2d:%2d:%02d\n", line, ptm->tm_year + 1900, ptm->tm_mon + 1,
                ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
        fflush(fp);
        sleep(1);
    }

    fclose(fp); 
    return 0;
}
