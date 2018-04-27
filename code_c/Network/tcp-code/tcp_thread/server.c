#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <unistd.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>

void handler_process(int signum)
{
    waitpid(-1,NULL,WNOHANG);
    printf("child exit is success!\n");
    return;
}
void *dealData(void *arg)
{
    int ret = 0;
    int fd = *(int *)arg;
    //子进程去处理数据的接收
    char buf[1024] = {0};
    while(1)
    {
        ret = recv(fd, buf, 1024, 0);
        if (ret == 0)
        {
            printf("client has shutdown\n");
            break;
        }

        printf("ret = %d, buf is %s\n", ret, buf);
        memset(buf, 0, 1024);
    }

    close(fd);


    return NULL;
}
int main(int argc, const char *argv[])
{
    pthread_t tid = 0;
    if (argc < 3)
    {
        fprintf(stderr, "Useage: %s ip port\n", argv[0]);
        return 0;
    }
    //对僵尸态子进程进行回收
    if(signal(SIGCHLD,handler_process) == SIG_ERR)
    {
        perror("Fail to signal");
        exit(EXIT_FAILURE);
    }

    //1.客户端创建支持tcp的socket
    int fd_tcp_s = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_tcp_s < 0)
    {
        perror("socket");
        return 0;
    }
    //2.服务器绑定IP地址和端口号
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = bind(fd_tcp_s, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        perror("connect");
        return 0;
    }
    //3.服务器设置监听
    ret = listen(fd_tcp_s, 10);
    if (ret < 0)
    {
        perror("listen");
        return 0;
    }
    
    while (1)
    {
        //4.阻塞接收用户的接入
        struct sockaddr_in clientAddr = {0};
        int len  = sizeof(clientAddr);
        int new_fd = accept(fd_tcp_s, (struct sockaddr*)&clientAddr, &len);
        if (new_fd < 0)
        {
            perror("accept");
            return 0;
        }
        printf("accept ip %s port %d connect\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        int ret = pthread_create(&tid, NULL, dealData, (void *)&new_fd);
        //将线程设置成分离的线程，交给系统来回收资源
        pthread_detach(tid);
    }
    close(fd_tcp_s);
    return 0;
}
