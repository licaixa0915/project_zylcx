#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
void *thread_fun(void *args)
{
    int fd = *((int *)args);
    char buf[1024] = {0};
    printf("fd = %d\n",fd);
    while(1)
    {
        if(read(fd, buf, 1024))
            printf("buf = %s\n", buf);
        sleep(1);
    }
    return NULL;
}
int main(int argc, const char *argv[])
{
    int fd = 0;
    fd = open("1.txt", O_RDWR | O_CREAT|O_TRUNC, 0666);
    if (fd  == -1)
    {
        perror("open");
        return 0;
    }
    pthread_t tid = 0;
    pthread_create(&tid, NULL,thread_fun,&fd);

    char buf[1024];
    printf("main fd = %d\n", fd);

    while(1)
    {
        gets(buf);
        if (strcmp(buf,"quit") == 0)
        {
            return 0;
        }
        write(fd, buf,strlen(buf)+1);
        lseek(fd, -(strlen(buf)+1),SEEK_CUR);
    }
    return 0;
}
