#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
int main(int argc, const char *argv[])
{
    int socketFd = 0; 
    struct sockaddr_in groupAddr = {0};
    int n = 0;
    char buf[1024] = {0};

    //1：创建socket
    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd < 0)
    {
        perror("socket");
        return 0;
    }

    //2：填充组播地址[224.0.0.1 – 239.255.255.255]
    groupAddr.sin_family = AF_INET;
    groupAddr.sin_port = htons(atoi(argv[2]));
    groupAddr.sin_addr.s_addr = inet_addr(argv[1]);

    //3：发送组播数据
    while (1)
    {
        printf("send>");
        fflush(stdout);
        scanf("%s", buf);
        n = sendto(socketFd, buf, strlen(buf), 0, (struct sockaddr*)&groupAddr, sizeof(groupAddr));
        if (n < 0)
        {
            perror("sendto");
            return 0;
        }
    }

    return 0;
}
