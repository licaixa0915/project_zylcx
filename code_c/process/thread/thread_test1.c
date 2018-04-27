#include <stdio.h>
#include <pthread.h>
struct arg
{
    int int_data;
    short short_data;
    char char_data;
};
int g_data = 10;
void * thread_fun(void *arg)
{
    while(1)
    {
        printf("int_dada = %d, short_data = %d, char_data = %d\n", ((struct arg*)arg)->int_data, 
            ((struct arg*)arg)->short_data, ((struct arg*)arg)->char_data);
        g_data++;
        sleep(1);
    }
    return NULL;
}
void * thread_fun1(void *arg)
{
    while(1)
    {
        printf("thread 1 data = %d, g_data = %d\n", *((int *)arg), g_data);
        *((int*)arg) = *((int*)arg) + 1; 
        sleep(1);
    }
    return NULL;
}
int main(int argc, const char *argv[])
{
    int ret = 0;
    pthread_t tid = 0;
    int arg = 10;
    int data = 0;
    struct arg stArg = {20,30,60};

    ret = pthread_create(&tid, NULL, thread_fun, &stArg);
    printf("ret = %d\n", ret);
    ret = pthread_create(&tid, NULL, thread_fun1, &data);
    while(1)
    {
        printf("main running data = %d\n", data);
        sleep(1);
    }
    return 0;
}
