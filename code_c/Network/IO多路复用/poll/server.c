#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <poll.h>
#include <string.h>
#include <netinet/in.h>

#define MAX_POLL_NUM 10

int main(int argc, const char *argv[])
{
    int ret = 0;
    struct sockaddr_in serverAddr = {0};
    int sockfd = 0;
    char buf[1024] = {0};
    int new_fd = 0;
    int n = 0;
    int i = 0;
    struct pollfd fds[MAX_POLL_NUM] = {0};
    
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

    fds[0].fd = sockfd;
    fds[0].events = POLLIN;
    n++;

    while (1)
    {
        ret = poll(fds, MAX_POLL_NUM, -1);
        if (ret < 0)
        {
            perror("poll");
            return 0;
        }
        else
        {
            for (i = 0; i < n; i++)
            {
                
                if (fds[i].revents == POLLIN)
                {
                    if (fds[i].fd == sockfd)
                    {
                        printf("have client connect\n");
                        //表示有客户端接入
                        int new_fd = accept(sockfd, NULL, NULL);
                        if (new_fd < 0)
                        {
                            perror("accept");
                            return 0;
                        }
                        fds[n].fd = new_fd;
                        fds[n].events = POLLIN;
                        n++;
                    }
                    else
                    {
                        //表示有客户端发消息过来
                        char buf[1024] = {0};
                        if(recv(fds[i].fd, buf, 1024, 0) <= 0)
                        {
                            printf("close client fd\n");
                            close(fds[i].fd);
                            fds[i].fd = 0;
                            fds[i].events = 0;
                        }
                        else
                        {
                            printf("recv:%s\n", buf);
                        }
                    }
                }
            }
        }
    }
    return 0;
}
