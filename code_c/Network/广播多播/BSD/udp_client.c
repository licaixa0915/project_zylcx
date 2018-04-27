#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>

int main(int argc, const char *argv[])
{
    int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return 0;
    }

    struct sockaddr_un clientAddr = {0};
    clientAddr.sun_family = AF_UNIX;
    strcpy(clientAddr.sun_path, argv[1]);   
    remove(argv[1]);
    int ret = bind(sockfd, (struct sockaddr*)&clientAddr, sizeof(clientAddr));
    if (ret < 0)
    {
        perror("bind");
        return 0;
    }
    
    struct sockaddr_un serverAddr = {0};
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, argv[2]);
    while (1)
    {
        char buf[1024] = {0};
        scanf("%s", buf);
        sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        bzero(buf, 1024);
        recvfrom(sockfd, buf, 1024, 0, NULL, NULL);
        printf("buf:%s\n", buf);
    }
    return 0;
}
