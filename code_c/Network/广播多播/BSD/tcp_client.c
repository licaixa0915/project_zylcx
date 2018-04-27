#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>

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
    strcpy(serverAddr.sun_path, argv[2]);   
    int new_fd = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (new_fd < 0)
    {
        perror("connect");
        return 0;
    }
    
    while (1)
    {
        char buf[1024] = {0};
        scanf("%s", buf);
        send(sockfd, buf, strlen(buf), 0);
        bzero(buf, 1024);
        recv(sockfd, buf, 1024, 0);
        printf("buf:%s\n", buf);
    }
    return 0;
}
