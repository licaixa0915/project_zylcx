#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <error.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    char buf[1024] = {0};
    /*创建socket*/
    if (argc < 3)
    {
        fprintf(stderr, "Useage: %s broadcast_ip broadcast_port\n", argv[1]);
        return 0;
    }

    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockFd < 0)
    {
        perror("socket");
        return 0;
    }

    //bind ip和port
    struct sockaddr_in thisAddr = {0};
    thisAddr.sin_family = AF_INET;
    thisAddr.sin_port = htons(atoi(argv[2]));
    thisAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = bind(sockFd, (struct sockaddr*)&thisAddr, sizeof(thisAddr)); 
    //发送消息给某个IP
    struct sockaddr_in srcAddr = {0};
    int len = sizeof(srcAddr);
    while (1)
    {
        recvfrom(sockFd, buf, sizeof(buf), 0, (struct sockaddr*)&srcAddr, &len);
        printf("msg from %s:%d is %s\n", inet_ntoa(srcAddr.sin_addr), htons(srcAddr.sin_port), buf);
        memset(buf, 0, 1024);
    }
        return 0;
}
