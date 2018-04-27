#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <poll.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/epoll.h>

#define MAX_RETEPOLL_NUM 10

int main(int argc, const char *argv[])
{
    int ret = 0;
    struct sockaddr_in serverAddr = {0};
    int sockfd = 0;
    char buf[1024] = {0};
    int epollfd = 0;
    int new_fd = 0;
    struct epoll_event ev[MAX_RETEPOLL_NUM] = {0};
    int i = 0;
    
    if  (argc < 3)
    {
        printf("Useage: %s serverip serverport\n", argv[0]);
        return 0;
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);    
    if (sockfd < 0)
    {
        perror("socket");
        return 0;
    }
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        perror("connect");
        return 0;
    }

    listen(sockfd, 10);
    epollfd = epoll_create(1);
    if (epollfd < 0)
    {
        perror("epoll_create");
        return 0;
    }
    struct epoll_event ep = {0};
    ep.events = EPOLLIN;
    ep.data.fd = sockfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ep);
    while (1)
    {
        ret = epoll_wait(epollfd, ev, MAX_RETEPOLL_NUM, -1);
        if (ret < 0)
        {
            perror("poll");
            return 0;
        }
        else
        {
            for (i = 0; i < ret; i++)
            {
                if (ev[i].data.fd == sockfd)
                {
                    printf("have client connect\n");
#if 0
                    //表示有客户端接入
                    int new_fd = accept(sockfd, NULL, NULL);
                    if (new_fd < 0)
                    {
                        perror("accept");
                        return 0;
                    }
                    ep.events = EPOLLIN;
                    ep.data.fd = new_fd;
                    epoll_ctl(epollfd, EPOLL_CTL_ADD, new_fd, &ep);
#endif
                }
                else
                {
                    //表示有客户端发消息过来
                    char buf[1024] = {0};
                    if(recv(ev[i].data.fd, buf, 1024, 0) <= 0)
                    {
                        printf("close client fd\n");
                        close(ev[i].data.fd);
                        ep.events = EPOLLIN;
                        ep.data.fd = new_fd;
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, new_fd, &ep);
                    }
                    else
                    {
                        printf("recv:%s\n", buf);
                    }
                
                }
            }
        }
    }
    return 0;
}
