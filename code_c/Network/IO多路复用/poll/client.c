#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    int ret = 0;
    struct sockaddr_in serverAddr = {0};
    int sockfd = 0;
    char buf[1024] = {0};

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
    ret = connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        perror("connect");
        return 0;
    }
    
    while (1)
    {
        scanf("%s", buf);
        send(sockfd, buf, strlen(buf), 0); 
    }
    return 0;
}
