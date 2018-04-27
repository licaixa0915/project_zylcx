#include <stdio.h>
extern void do_client_task(int tcpFd);
extern void do_client_task(int tcpFd);
int main (int argc, const char *argv[])
{
    int socketFd = 0;
    int ret = 0;

    if (argc < 3)
    {
        fprintf(stderr, "Useage:%s serverip serverport\n", argv[0]);
        return 0;
    }

    //初始化客户端的tcp通信相关的注册等
    ret = init_tcp(argv[1], argv[2], &socketFd);
    if (ret != 0)
    {
        printf("socket error\n");
        return 0;
    }
    
    //处理客户端的用户请求
    do_client_task(socketFd);
	return 0;
}


