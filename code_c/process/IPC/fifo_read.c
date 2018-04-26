#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    if (mkfifo("fifo1",0777) < 0 && errno != EEXIST)
    {
        perror("mkinfo");
        exit(-1);
    }
    int fd = 0;
    fd = open("fifo1", O_RDONLY|O_NONBLOCK);
    if (fd < 0)
    {
        perror("open");
        exit(-1);
    }
    char buf [1024] = {0};
    while(1)
    {
        read(fd, buf, 1024);
        printf("output:%s\n", buf);
        usleep(200);
    }
    return 0;
}
