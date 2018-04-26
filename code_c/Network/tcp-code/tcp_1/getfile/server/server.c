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
#include <errno.h>
#define TRANSFILE "video.mp4"
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
    int ret = bind(fd_tcp, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        perror("bind");
        return 0;
    }

    ret = listen(fd_tcp, 10);
    if (ret < 0)
    {
        perror("listen");
        return 0;
    }
    struct sockaddr_in clientAddr = {0};
    int len = sizeof(clientAddr);
    int new_fd = accept(fd_tcp, (struct sockaddr*)&clientAddr, &len);
    if (new_fd < 0)
    {
        perror("accpet");
        return 0;
    }
    printf("accept ip %s port %d connect", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    int n = send(new_fd, TRANSFILE, strlen(TRANSFILE), 0); 
    if (n < 0)
    {
        perror("ensd");
        return 0;
    }

    int fd = open(TRANSFILE, O_RDONLY); 
    if (fd < 0)
    {
        perror("open");
        return 0;
    }
    //sleep(1);
    char buf[1024] = {0};
    while (1)
    {
        memset(buf, 0, 1024);
        n = 0;
        n = read(fd, buf, 10);
        printf("n:%d, buf:%s\n", n, buf); 
        ret = send(new_fd, buf, n, 0);
        if (ret < 0)
        {
            perror("recv");
            return 0;
        }
    }

    return 0;
}
