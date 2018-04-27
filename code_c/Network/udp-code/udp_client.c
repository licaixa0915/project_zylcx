#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, const char *argv[])
{    
    /* 1�������׽���socket */
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket <  0)
    {
        perror("socket");
        return 0;
    }
    printf("socket\n");
    /* 2���������ݸ������� */
    char buf[2048] = {0};
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("192.168.3.27");
    char buf1[1024] = {0};
    struct sockaddr_in clientAddr = {0};
    int len = sizeof(struct sockaddr);
    int ret = 0;
    while (1)
    {
        /*�����û����ݷ��͸�������*/
        scanf("%s", buf);
        int ret = sendto(udp_socket, buf, 2048, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));    
        if (ret < 0)
        {
            perror("sendto");
            return 0;
        }
        /* ���շ��������ͻ��������� */
        ret = recvfrom(udp_socket, buf1, sizeof(buf1), 0, (struct sockaddr*)&clientAddr, &len); 
        printf("ret = %d\n", ret);
        printf("recv from ip: %s:%s\n",inet_ntoa(clientAddr.sin_addr), buf1);
        memset(buf1, 0, sizeof(buf1));
    }
    return 0;
}