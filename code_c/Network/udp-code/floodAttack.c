#include <sys/socket.h>
#include <netinet/in.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
	//1 创建socket文件
	int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	//                  使用的是ipv4
	//                         告诉我们使用的UDP
	if(udp_socket < 0)
	{
		perror("create socket err:");
		exit(-1);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	//htons其主要作用就是把34444转换为 网络字节序 格式
	//host to network
	//由主机字节序 转换为 网络字节序
	addr.sin_port = htons(53);
	addr.sin_addr.s_addr = inet_addr("192.168.3.1");

    while(1)
    {	//2 调用sendto函数 来实现发送数据报
	    int ret = sendto(
					udp_socket,
					"hello\n",
					6,
					0,
					(struct sockaddr *)&addr,
					sizeof(addr)
					);
    }
	return 0;
}



