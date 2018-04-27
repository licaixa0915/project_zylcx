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

    //2：加入到组播的IP地址中
    struct ip_mreq mreq = {0};
    mreq.imr_multiaddr.s_addr = inet_addr(argv[1]);
    mreq.imr_interface.s_addr = INADDR_ANY;
    int ret = setsockopt(socketFd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

    //3: bind到组播中
    groupAddr.sin_family = AF_INET;
    groupAddr.sin_port = htons(atoi(argv[2]));
    groupAddr.sin_addr.s_addr = inet_addr(argv[1]);

    if (bind(socketFd, (struct sockaddr*)&groupAddr, sizeof(groupAddr)) < 0)
    {
        perror("bind");
        return 0;
    }

    while (1)
    {
        //4：接收组播数据
        n = recvfrom(socketFd, buf, sizeof(buf), 0, NULL, 0);
        if (n < 0)
        {
            perror("recvfrom");
            return 0;
        }
        printf("recvfrom group msg:%s\n", buf);
    }

    return 0;
}
