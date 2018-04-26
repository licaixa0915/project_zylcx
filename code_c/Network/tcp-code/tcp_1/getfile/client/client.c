#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Useage: %s ip port\n", argv[0]);
        return 0;
    }

    int fd_tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_tcp < 0)
    {
        perror("socket");
        return 0;
    }
    
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[2]));
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(fd_tcp, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        perror("connect");
        return 0;
    }

    char buf[1024] = {0};
    ret = recv(fd_tcp, buf, 1024, 0);
    if (ret < 0)
    {
        perror("recv");
        return 0;
    }

    printf("ret:%d, buf:%s\n", ret, buf);

    int fd = open(buf, O_WRONLY|O_CREAT|O_TRUNC, 0666); 
    if (fd < 0)
    {
        perror("open");
        return 0;
    }

    memset(buf, 0, 1024);
    while (1)
    {
        ret = recv(fd_tcp, buf, 1024, 0);
        if (ret < 0)
        {
            perror("recv");
            return 0;
        }
        printf("ret = %d, buf = %s\n", ret, buf);
        
        write(fd, buf, ret);
        memset(buf, 0, 1024);
    }

    return 0;
}
