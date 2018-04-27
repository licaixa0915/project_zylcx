#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <netinet/ip.h>
#include "head.h"
#include <strings.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

#define RET_OK   0
#define RET_ERR -1
//用户输入指令，供大家选择
enum
{
    LOGIN    = 1,  //登陆
    REGISTER = 2,  //注册
    QUIT     = 3,  //退出
    QUERY    = 1,  //查询单词
    HISTORY  = 2,  //查询历史
};
#define HISTORY_FILE ".history"

/*******************************************
 *函数名称:int init_tcp(char *ip, char *port)
 *函数功能:初始化TCP通信需要的接口
 *函数入参:@ip     服务器的ip地址
 *         @port   服务器做电子词典的服务器
 *函数出参:@pFd    创建socket产生的文件描述符
 *函数返回值:int 成功返回RET_OK, 失败返回RET_ERR
 *作者:licaixia
 *******************************************/
int init_tcp(char *ip, char *strPort, int *pFd)
{
    int port = 0;
    int socketFd = 0;
    struct sockaddr_in serverAddr = {0};
    
    //将用户输入的字符串类型的端口号转换成整数类型的
    port = atoi(strPort);
    //创建tcp socket
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0)
    {
        perror("socket");
        return RET_ERR;
    }

    //链接数据库
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    if (connect(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("connect");
        return RET_ERR;
    }
    *pFd = socketFd;
    return RET_OK;
}

//用户登录注册的界面
void help_info_logreg()
{
	printf("\t-----------------------------------------------\n");
	printf("\t|               hqyj   在线辞典                |\n");
	printf("\t|版本:0.0.1                                    |\n");
	printf("\t|作者:lcx老师                                  |\n");
	printf("\t|功能:                                         |\n");
	printf("\t|    [1] 登录                                  |\n");
	printf("\t|    [2] 注册                                  |\n");
	printf("\t|    [3] 退出                                  |\n");
	printf("\t|注意:用户只有登录成功后才能进入查单词界面     |\n");
	printf("\t------------------------------------------------\n");
	printf("\t请输入您的选择>");
	return;
}
//用户提示界面2
void help_info_search()
{
	printf("\t-----------------------------------------------\n");
	printf("\t|     欢迎进入单词查询系统，很高兴为您服务     |\n");
	printf("\t|版本:0.0.1                                    |\n");
	printf("\t|作者:lcx老师                                  |\n");
	printf("\t|功能:                                         |\n");
	printf("\t|    [1] 查单词                                |\n");
	printf("\t|    [2] 查询历史记录                          |\n");
	printf("\t|    [3] 退出                                  |\n");
	printf("\t|注意:用户只有登录成功后才能进入查单词界面     |\n");
	printf("\t------------------------------------------------\n");
	printf("\t请输入您的选择>");
	return;
}


/*******************************************
 *函数名称:void do_send(MHEAD_T *pstHead, int sockfd)
 *函数功能:处理客户端的发送消息
 *函数入参:@sockfd  客户端用于通信的socket fd
 *         @pstHead 用于通信的自定义协议头
 *函数出参:无
 *函数返回值:void
 *作者:licaixia
 *******************************************/
void do_send(MHEAD_T *pstHead, int sockfd)
{
    //发送前统一转字节序,char类型的的不需要转
    pstHead->size = htonl(pstHead->size);
    pstHead->type = htonl(pstHead->type);

    //发给服务器端
    if(send(sockfd, pstHead, sizeof(MHEAD_T),0) < 0)
    {
        perror("Fail to send");
        exit(EXIT_FAILURE);
    }
    //printf("send ok\n");
    return;
}

void do_history(char * msg, int type)
{
    FILE *fp = NULL;
    char buf[1024] = {0};

    fp = fopen(HISTORY_FILE, "a+");
    if (fp == NULL)
    {
        perror("fopen");
        return;
    }
    printf("type = %d\n", type);
    if (type == 0)
    {
        time_t tim = time(NULL);
        struct tm *ptm = localtime(&tim);
    
        sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d\n%s\n\n",ptm->tm_year + 1900, 
            ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_sec, ptm->tm_sec, msg);
        printf("buf:%s", buf); 
        fputs(buf, fp);
    }
    else
    {
        char buf[1024] = {0};
        while (fgets(buf, 1024, fp))
        {
            printf("%s", buf);       
        }
    }
    fclose(fp);
    return;
}
/*******************************************
 *函数名称:void do_recv(int sockfd)
 *函数功能:处理客户端的接收消息
 *函数入参:@sockfd  客户端用于通信的socket fd
 *函数出参:无
 *函数返回值:int 成功返回RET_OK  失败返回RET_ERR
 *作者:licaixia
 *******************************************/
int do_recv(int sockfd)
{
    SHEAD_T stRecvHead = {0};

    int count = 0;
    int n = 0;

    //接收协议头
    while(1)
    {
        n = recv(sockfd, &stRecvHead + count,sizeof(SHEAD_T) - count,0);
        if(n <= 0)
        {
            perror("Fail to recv");                                                 
            exit(EXIT_FAILURE);                  
        }     
        count += n;                                                                          
        if(count == sizeof(SHEAD_T))
            break;                                                                   
    }
    stRecvHead.retType = ntohl(stRecvHead.retType);
    printf("\n%s\n", stRecvHead.result);
    if (stRecvHead.retType != USER_SUCCESS)
    {
        return RET_ERR;
    }

    if (ntohl(stRecvHead.msgType) == USER_WORD)
    {
        //将数据填充到.history文件中
        do_history(stRecvHead.result, 0);
    }

    return RET_OK;
}
/*******************************************
 *函数名称:void do_client_task(int tcpFd)
 *函数功能:处理客户端的请求
 *函数入参:@tcpFd  客户端用于通信的socket fd
 *函数出参:无
 *函数返回值:void
 *作者:licaixia
 *******************************************/
void do_client_task(int tcpFd)
{
    int iFunNo = 0;
    MHEAD_T stHead = {0};
    int ret = 0;

    FILE *fp = fopen(HISTORY_FILE, "w");
    fclose(fp);
    while (1)
    {
        //界面
        help_info_logreg();
        
        //接收用户的输入，并进行处理
        scanf("%d", &iFunNo);
        while (!((iFunNo <= QUIT) && (iFunNo >= LOGIN)))
        {
            scanf("%d", &iFunNo);
            fprintf(stderr, "请重新输入合法的编号(%d-%d):", LOGIN, QUIT);
        }
        
        switch (iFunNo)
        {
            case LOGIN:
                printf("您正在登录，请输入用户名和密码>\n");
                printf("input username:");
                scanf("%s", stHead.username);
                printf("input password:");
                scanf("%s", stHead.password);
                stHead.type = USER_LOGIN;
                break;
            case REGISTER:
                printf("您正在注册，请输入用户名和密码>\n");
                printf("input username:");
                scanf("%s", stHead.username);
                printf("input password:");
                scanf("%s", stHead.password);
                stHead.type = USER_REGISTER;
                break;
            case QUIT:
                exit(0);
                break;
            default:
                fprintf(stderr, "输入功能号错误，请重新输入!\n");
                continue;
        }

        stHead.size = sizeof(MHEAD_T);

        do_send(&stHead, tcpFd);
        
        ret = do_recv(tcpFd);
        if (ret != RET_OK)
        {
            continue;
        }
        while (1)
        {
            MHEAD_T stHead = {0};
            //界面
            help_info_search();
        
            //接收用户的输入，并进行处理
            scanf("%d", &iFunNo);
            while (!((iFunNo <= QUIT) && (iFunNo >= QUERY)))
            {
                scanf("%d", &iFunNo);
                fprintf(stderr, "请重新输入合法的编号(%d/%d):", QUERY, QUIT);
            }

            switch (iFunNo)
            {
                case QUERY:
                    printf("请输入您需要查询的单词>\n");
                    scanf("%s", stHead.word);
                    stHead.type = USER_WORD;
                    break;
                case HISTORY:
                    do_history(NULL, 1);
                    continue;
                case QUIT:
                    exit(0);
                    break;
                default:
                    fprintf(stderr, "您的输入不合法,请重新选择!\n");
                    continue;
            }

            stHead.size = sizeof(MHEAD_T);

            do_send(&stHead, tcpFd);
            do_recv(tcpFd);
        }
    }
    return;
}
