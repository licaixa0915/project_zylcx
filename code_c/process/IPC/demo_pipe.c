#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    pid_t pid = 0;
    int fd[2] = {0};
    int ret = 0;
    //父进程创建无名管道
    ret = pipe(fd);
    if (0 != ret)
    {
        perror("pipe");
        return 0;
    }
    //父进程通过fork创建子进程
    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        return 0;
    }
    else if(0 == pid)
    {
        //子进程关闭读端
        close(fd[0]);
        char buf[1024] = {0};
        while(1)
        {
            //子进程不停的接收用户的输入，写入到管道中
            printf("input:");
            gets(buf);
            write(fd[1], buf, strlen(buf)+1);
            usleep(200);
        }
    }
    else
    {
        //父进程关闭写端
        close(fd[1]);
        char bufr[1024] = {0};
        while(1)
        {
            //父进程不停的从管道中读出数据
            read(fd[0], bufr, 1024);
            printf("output:%s\n",bufr);
        }
    }
    return 0;
}
