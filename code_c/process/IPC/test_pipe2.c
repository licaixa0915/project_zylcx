#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, const char *argv[])
{
    pid_t pid = -1;
    int pipefd1[2] = {0};
    int pipefd2[2] = {0};
    int ret = 0;
    char buf[10] = {0};
    FILE *fp = NULL;

    fp = fopen("1.txt", "w+");
    ret = pipe(pipefd1);
    if (0 != ret)
    {
        perror("pipe");
        return 0;
    }
    ret = pipe(pipefd2);
    if (0 != ret)
    {
        perror("pipe");
        return 0;
    }

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        return 0;
    }
    else if (0 == pid)
    {
        time_t tim;
        struct tm *ptm;
        close(pipefd1[1]);//关闭写端
        close(pipefd2[0]);
        char buf1[1024];
        while(1)
        {
            read(pipefd1[0], buf, 10);
            tim = time(NULL);
            ptm = localtime(&tim);
            fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d", ptm->tm_year+1990, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
            fflush(fp); 
            write(pipefd2[1], "a", 1);
        }
    }
    else
    {
        close(pipefd1[0]);//关闭读端
        close(pipefd2[1]);
        int n = 0;
        while(1)
        {
            fprintf(fp,"%d, <", n++);
            fflush(fp); 
            write(pipefd1[1], "a", 1);
            read(pipefd2[0], buf, 10);
            fprintf(fp, ">\n");
            fflush(fp); 
            sleep(1);
        }
    }

    return 0;
}
