#include <stdio.h>
#include <pthread.h>

int n = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_n_ready=PTHREAD_COND_INITIALIZER;

void * thread_add(void *arg)
{
    printf("hello\n");
    while(1)
    {
        pthread_mutex_lock(&lock);
        n++;
        pthread_mutex_unlock(&lock);
        if (n>100)
            pthread_cond_signal(&cond_n_ready);
        usleep(1000);
    }
    pthread_exit(NULL);
}

void * thread_clr(void *arg)
{
    printf("world\n");
    while(1)
    {
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond_n_ready, &lock);
        printf("n = %d\n", n);
        n = 0;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

int main(int argc, const char *argv[])
{
    int ret = 0;
    pthread_t tid[4];
    int i = 0;

    for (i = 0; i < 3; i++)
    {
        ret = pthread_create(&tid[i], NULL, thread_add, NULL);
        if (0 != ret)
        {
            perror("pthread_create");
            return 0;
        }
    }

    ret = pthread_create(&tid[3], NULL, thread_clr, NULL);
    if (0 != ret)
    {
        perror("pthread_create");
        return 0;
    }
    
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    return 0;
}
