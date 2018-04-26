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
    if (argc < 3)
    {
        fprintf(stderr, "Useage: %s ip port\n", argv[0]);
        return 0;
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
    char buf[1024] = {0};
    while(1)
    {
        ret = recv(new_fd, buf, 1024, 0);
        if (ret == 0)
        {
            printf("client has shutdown, server will exit\n");
            break;
        }

        printf("ret = %d, buf is %s\n", ret, buf);
        memset(buf, 0, 1024);
    }

    close(new_fd);
    close(fd_tcp_s);
    return 0;
}
