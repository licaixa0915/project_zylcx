#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

int main(int argc, const char *argv[])
{
    char buf[1024] = {0};
    /*创建socket*/
    int sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockFd < 0)
    {
        perror("socket");
        return 0;
    }
    int broadcast = 1;
    int ret = setsockopt(sockFd, SOL_SOCKET, SO_BROADCAST, (void *)&broadcast,sizeof(int));
    printf("ret = %d, \n", ret);
    perror("setopt");

    //发送消息给某个IP
    struct sockaddr_in dstAddr = {0};
    dstAddr.sin_family = AF_INET;
    dstAddr.sin_port = htons(atoi(argv[2]));
    dstAddr.sin_addr.s_addr = inet_addr(argv[1]);
    while (1)
    {
        printf("send msg>");
        scanf("%s", buf);
        ret = sendto(sockFd, buf, strlen(buf), 0, (struct sockaddr*)&dstAddr, sizeof(dstAddr));
        printf("ret = %d\n", ret);
    }
        return 0;
}
