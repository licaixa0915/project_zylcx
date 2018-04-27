#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, const char *argv[])
{    
    int ret = 0;
    /* 1�������׽���socket */
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket <  0)
    {
        perror("socket");
        return 0;
    }
    /* 2���������ݸ������� */
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family  = AF_INET;//��ַЭ����
    serverAddr.sin_port    = htons(8888);//�˿ں�
    serverAddr.sin_addr.s_addr = inet_addr("192.168.3.27");
    if(bind(udp_socket,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0)
    {
        perror("bind fail");
        return 0;
    }
    char buf1[1024] = {0};
    char buf[1024] = {0};
    struct sockaddr_in clientAddr = {0};
    int len = sizeof(struct sockaddr);
    while (1)
    {
        /*�����ȴ��ͻ��˷������ݹ���*/
        ret = recvfrom(udp_socket, buf1, sizeof(buf1), 0, (struct sockaddr*)&clientAddr, &len); 
        printf("recv from ip: %s:%s\n",inet_ntoa(clientAddr.sin_addr), buf1);
        sprintf(buf, "server recv %s", buf1);
        /*���ͻ��˷��͹���������ƴ��һ���ٷ���ȥ */
        ret = sendto(udp_socket, buf, 2048, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));    
        //ret = sendto(udp_socket, buf, 2048, 0, NULL, sizeof(clientAddr));    
        //int ret = sendto(udp_socket, buf, 2048, 0, NULL, sizeof(serverAddr));    
        if (ret < 0)
        {
            perror("sendto");
            return 0;
        }

        memset(buf1, 0, sizeof(buf1));
        memset(buf, 0, sizeof(buf));
    }
    return 0;
}