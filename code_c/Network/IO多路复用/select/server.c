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
    
    fd_set fd_box;
    fd_set fd_box_bak;

    FD_ZERO(&fd_box);
    FD_ZERO(&fd_box_bak);

    FD_SET(fd_tcp_s, &fd_box_bak);
    int max1 = fd_tcp_s + 1;
    int new_fd = 0;
    struct sockaddr_in clientAddr = {0};
    int len = sizeof(clientAddr);
    char buf[1024] = {0};
    int i = 0 ;
    while (1)
    {
        fd_box = fd_box_bak;
        ret = select(max1, &fd_box, NULL, NULL, NULL);
        if (ret < 0)
        {
            perror("select");
            return 0;
        }

        if (ret > 0)
        {
            for (i = 0; i < max1; i++)
            {
                if (FD_ISSET(i, &fd_box))
                {
                    if (i == fd_tcp_s)
                    {
                        //表示有新的客户端的接入，出发了fd_tcp_s的异动
                        new_fd = accept(fd_tcp_s, (struct sockaddr*)&clientAddr, &len);
                        printf("accept ip %s port %d connect\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
                        FD_SET(new_fd, &fd_box_bak);
                        max1 = new_fd + 1;

                    }
                    else
                    {
                        memset(buf, 0, 1024);
                        //表示其他的有客户端发送数据过来
                        int n = recv(i, buf, sizeof(buf), 0);
                        if (0 == n)
                        {
                            FD_CLR(i, &fd_box_bak);
                            close(i);
                            printf("%d closed\n", i);
                        }
                        else
                        {
                            printf("fd %d send msg:%s\n", i, buf);
                        }
                    }
                }
            }
        }
    }

    close(fd_tcp_s);
    return 0;
}
