#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    pid_t pid = -1;
    int pipefd1[2] = {0};
    int pipefd2[2] = {0};
    int ret = 0;

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
        char buf[1024] = {0};
        close(pipefd1[0]);//关闭读端
        printf("input:");
        gets(buf);
        write(pipefd1[1], buf, strlen(buf)+1);
        close(pipefd2[1]);
        char buf3[1024] = {0};
        read(pipefd2[0], buf3, 1024);
        printf("%s", buf3);
    }
    else
    {
        close(pipefd1[1]);//关闭写端
        char buf1[1024] = {0};
        read(pipefd1[0], buf1, 1024);
        printf("output:%s\n", buf1);
        char buf2[1024] = {0};
        sprintf(buf2,"child process recived string:%s\n", buf1);
        close(pipefd2[0]);
        write(pipefd2[1], buf2, strlen(buf2)+1);
    }

    return 0;
}
