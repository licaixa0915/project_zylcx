#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, const char *argv[])
{
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return 0;
    }

    struct sockaddr_un serverAddr = {0};
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, argv[1]);
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) 
    {
        perror("bind");
        return 0;
    }
   
    listen(sockfd, 10);
    
    int new_fd = accept(sockfd, NULL, 0);
    if (new_fd < 0)
    {
        perror("accept");
        return 0;
    }
    printf("new_fd:%d\n", new_fd);
    while (1)
    {
        char buf[1024] = {0};
        int ret = recv(new_fd, buf, 1024, 0);
        printf("ret = %d, buf:%s\n", ret, buf);
        sleep(1);
        send(new_fd, buf, strlen(buf), 0);
    }

    return 0;
}
