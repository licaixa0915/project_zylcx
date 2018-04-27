#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <unistd.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
int main(int argc, const char *argv[])
{
    
    int i = 0;
    if (argc < 3)
    {
        fprintf(stderr, "Useage: %s ip port\n", argv[0]);
        return 0;
    }

    //1.客户端创建支持tcp的socket
    int fd_tcp_c = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_tcp_c < 0)
    {
        perror("socket");
        return 0;
    }
    printf("fd_tcp_c:%d\n", fd_tcp_c);
    //2.连接服务器
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(fd_tcp_c, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        perror("connect");
        return 0;
    }
    printf("connect\n");
    //3.发送数据给服务器
    char bufS[1024] = {0};
    while (1)
    {
        scanf("%s", bufS);
        //ret = send(fd_tcp_c, bufS, strlen(bufS), 0);
        //ret = send(fd_tcp_c, bufS, strlen(bufS), 0, NULL, 0);
        int ret = write(fd_tcp_c, bufS, strlen(bufS));
        if (ret < 0)
        {
            perror("send");
            return 0;
        }
        memset(bufS, 0, sizeof(bufS));
    }

    return 0;
}
