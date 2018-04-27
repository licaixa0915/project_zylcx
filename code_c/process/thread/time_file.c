#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
sem_t sem_A;
sem_t sem_B;
void *pthread_writeN(void *argv)
{
    int fd = *((int*)argv);
    int n = 0;

    char buf[1024] = {0};
    while(1)
    {
        sem_wait(&sem_A);
        sprintf(buf,"%d, ",++n);
        write(fd, buf, strlen(buf));
        sleep(1);
        sem_post(&sem_B);
    }
}

void *pthread_writeTime(void *argv)
{
    char buf[1024] = {0};
    int fd = *((int*)argv);
    time_t tm = 0;
    struct tm *ptm = NULL;
    while(1)
    {
        sem_wait(&sem_B);
        tm = time(NULL);
        ptm = localtime(&tm);
        sprintf(buf,"%d-%02d-%02d %02d:%02d:%02d\n", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
        write(fd, buf, strlen(buf));
        sem_post(&sem_A);
        sleep(1);
    }
}

int main(int argc, const char *argv[])
{
    pthread_t tid1,tid2;
    int fd = open("1.txt", O_RDWR|O_CREAT|O_TRUNC);
    if (-1 == fd)
    {
        perror("open");
        return 0;
    }
    sem_init(&sem_A,0,1);
    sem_init(&sem_B,0,0);
    int ret = pthread_create(&tid1, NULL,pthread_writeN, &fd);
    if (0 != ret)
    {
        printf("create thread failed\n");
        return 0;
    }
    printf("read tid is %ld\n", tid1);
    
    ret = pthread_create(&tid2, NULL,pthread_writeTime, &fd);
    if (0 != ret)
    {
        printf("create thread failed\n");
        return 0;
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
