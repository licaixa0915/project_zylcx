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

    struct sockaddr_un serverAddr = {0};
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, argv[1]);   
    int ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        perror("bind");
        return 0;
    }
    
    struct sockaddr_un clientAddr = {0};
    int len = sizeof(clientAddr);
    while (1)
    {
        char buf[1024] = {0};
        recvfrom(sockfd, buf, 1024, 0, (struct sockaddr*)&clientAddr, &len);
        printf("clientAddr.path:%s\n", clientAddr.sun_path);
        printf("buf:%s\n", buf);
        sendto(sockfd, buf, 1024, 0, (struct sockaddr*)&clientAddr,len);
    }
    return 0;
}
