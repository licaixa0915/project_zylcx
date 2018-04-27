#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sqlite3.h>
#include <signal.h>
#include "head.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <strings.h>
#include <string.h>

#define RET_OK   0
#define RET_ERR -1
void signal_handler(int signum)
{
    waitpid(-1,NULL,WNOHANG);
    return;
}
/*******************************************
 *函数名称:int init_tcp(char *ip, char *port)
 *函数功能:初始化TCP通信需要的接口
 *函数入参:@ip     服务器的ip地址
 *         @port   服务器做电子词典的服务器
 *函数出参:@pFd    创建socket产生的文件描述符
 *函数返回值:int 成功返回RET_OK, 失败返回RET_ERR
 *作者:licaixia
 *******************************************/
int init_tcp(char *ip, char *port, int *pSockfd)
{
    int sockfd = -1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return RET_ERR;
    }
    
    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port));
    serverAddr.sin_addr.s_addr = inet_addr(ip);
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind");
        return RET_ERR;
    }
    
    listen(sockfd, 10);
    *pSockfd = sockfd;
    return RET_OK;
}

int do_register(int sockfd,sqlite3 *pdb,char *_username,char *_password)
{
	char *errmsg;
	char buf[1024];
	char **dbresult;
	int nrow,ncolumn;
	char sql[1024] = {0};
	MHEAD_T *head = (MHEAD_T *)buf;	

	sprintf(sql,"select * from user_table where NAME='%s';",_username);
    printf("sql:%s\n", sql);
	if(sqlite3_get_table(pdb,sql,&dbresult,&nrow,&ncolumn,&errmsg) != 0)
	{
		fprintf(stderr,"sqlite3 get table error : %s.\n",errmsg);
		exit(EXIT_FAILURE);
	}

    SHEAD_T stServerHead = {0};
	stServerHead.msgType = htonl(USER_REGISTER);
	//没有这样的用户名
	if(nrow == 0)
	{
		//录入数据库
		bzero(sql,sizeof(sql));
		sprintf(sql,"insert into user_table values('%s','%s');",_username,_password);
		EXEC_SQL(pdb,sql,errmsg);
		
		printf("register ok ........\n");

		stServerHead.retType = htonl(USER_SUCCESS);
        sprintf(stServerHead.result, "恭喜您，注册成功!");
		if(send(sockfd,&stServerHead,sizeof(SHEAD_T),0) < 0)
		{
			perror("Fail to send");
			exit(EXIT_FAILURE);
		}
	//注册失败，用户名存在
	}
    else
    {
		stServerHead.retType = htonl(USER_FAILURE);
        sprintf(stServerHead.result, "很遗憾，这个用户名已经被其它用户注册过了，请重新注册!");
		if(send(sockfd, &stServerHead, sizeof(SHEAD_T),0) < 0)
		{
			perror("Fail to send");
			exit(EXIT_FAILURE);
		}
		//表示未知	
		printf("???????\n");
	}
	//插入到数据库之后，释放dbresult结果
	sqlite3_free_table(dbresult);
	return 0;
}

int do_login(int new_fd,sqlite3 *pdb,char *_username,char *_password)
{
	char *errmsg;
	char buf[1024];
	char **dbresult;
	int nrow,ncolumn;
	char sql[1024] = {0};
	MHEAD_T *head = (MHEAD_T *)buf;	

	sprintf(sql,"select * from user_table where NAME='%s' and PASSWORD='%s';",_username,_password);
	
    printf("sql:%s\n", sql);
	if(sqlite3_get_table(pdb,sql,&dbresult,&nrow,&ncolumn,&errmsg) != 0)
	{
		fprintf(stderr,"sqlite3 get table error : %s.\n",errmsg);
		exit(EXIT_FAILURE);
	}

    SHEAD_T stServerHead = {0};
	stServerHead.msgType = htonl(USER_LOGIN);
	//没有这样的用户名
	if(nrow == 0)
	{	
		printf(" 没有这样的用户 ........\n");

		stServerHead.retType = htonl(USER_FAILURE);
        sprintf(stServerHead.result, "登录失败，用户名不存在或者密码错误，请重新登录!");
		if(send(new_fd,&stServerHead,sizeof(SHEAD_T),0) < 0)
		{
			perror("Fail to send");
			exit(EXIT_FAILURE);
		}
	//用户存在
	}
    else
    {

		stServerHead.retType = htonl(USER_SUCCESS);
        sprintf(stServerHead.result, "登录成功，您可以使用电子词典查询功能了!");
		if(send(new_fd, &stServerHead, sizeof(SHEAD_T),0) < 0)
		{
			perror("Fail to send");
			exit(EXIT_FAILURE);
		}
		//表示未知	
		printf("login ok\n");
	}
	//插入到数据库之后，释放dbresult结果
	sqlite3_free_table(dbresult);
	return 0;
}
void backward_move(char *s)
{
	char *p = s + strlen(s);

	//单引号之后的字符，依次向后偏移1位
	while(p != s)
	{
		*(p + 1) = *p;	
		p--;
	}
	//一个单引号变成两个单引号
	*(p + 1) = *p;
	return;
}
int do_word(int new_fd,sqlite3 *pdb,char * pword)
{
	printf("enter do_word is success\n");
	char *errmsg;
	char buf[1024];
	char **dbresult;
	int nrow,ncolumn;
	char sql[1024] = {0};
	MHEAD_T *head = (MHEAD_T *)buf;
	char *p = pword + strlen(pword);
		
	while(p != pword)
	{
		if(*p == '\'')	
		{
			backward_move(p); //找到引号的位置	
		}
		p--;
	}

	sprintf(sql,"select * from dict_table where word='%s';",pword);
    printf("sql:%s\n", sql);
	if(sqlite3_get_table(pdb,sql,&dbresult,&nrow,&ncolumn,&errmsg) != 0)
	{
		fprintf(stderr,"sqlite3 get table error : %s.\n",errmsg);
		exit(EXIT_FAILURE);
	}
	SHEAD_T stServerHead = {0};
	stServerHead.msgType = htonl(USER_WORD);
	//没有这样的单词
	if(nrow == 0)
	{	
		stServerHead.retType = htonl(USER_FAILURE);
		sprintf(stServerHead.result, "没有这个单词!");
		if(send(new_fd,&stServerHead,sizeof(SHEAD_T),0) < 0)
		{
			perror("Fail to send");
			exit(EXIT_FAILURE);
		}

	//单词存在
	}
	else
	{
	
		//告诉客户端单词存在
		stServerHead.retType = htonl(USER_SUCCESS);
		sprintf(stServerHead.result,"%s\t%s", dbresult[ncolumn], dbresult[ncolumn+1]);
		printf("ret:%s\n", stServerHead.result);

		if(send(new_fd,&stServerHead,sizeof(SHEAD_T),0) < 0)
		{
			perror("Fail to send");
			exit(EXIT_FAILURE);
		}
	}

	sqlite3_free_table(dbresult);
	
	return 0;
}

/*******************************************
 *函数名称:void do_client(int sockfd, struct sqlite3 *pdb)
 *函数功能:处理客户端发过来的消息
 *函数入参:@sockfd 用于tcp连接的文件描述符
 *         @pdb    数据库句柄
 *函数出参:无
 *函数返回值:void
 *作者:licaixia
 *******************************************/
void do_client(int new_fd, struct sqlite3 *pdb)
{
    int n;
    int count = 0;
    char buf[1024];
    MHEAD_T *head = (MHEAD_T *)buf;	

    while (1)
    {
    	count = 0;
	//接收协议头
	while(1)
	{
	    n = recv(new_fd,buf + count,sizeof(MHEAD_T) - count,0);
	    if(n <= 0)
	    {
            close();
		    exit(EXIT_FAILURE);
	    }
        printf("recv ok\n");
	    count += n;
	    if(count == sizeof(MHEAD_T))
		break;
	}
    head->type = ntohl(head->type);
    printf("type=%d\n", head->type);
    switch(head->type)
	{
		case USER_REGISTER:
			do_register(new_fd,pdb,head->username,head->password);	
			break;
		
		case USER_LOGIN:
			do_login(new_fd,pdb,head->username,head->password);
			break;

		case USER_WORD:
			do_word(new_fd,pdb,head->word);
			break;

		defalut:
			exit(EXIT_SUCCESS);
	}	
	
    }
    return;
}

int main(int argc, char *argv[])
{
    int sockfd = 0;
    int new_fd = 0;
    struct sqlite3 *pdb = NULL;
    
    int ret = init_tcp(argv[1], argv[2], &sockfd);
    if (ret < 0)
    {
        fprintf(stderr, "init_tcp error\n");
        return 0;
    }
    if(signal(SIGCHLD,signal_handler) == SIG_ERR)
    {
        perror("Fail to signal");
        exit(EXIT_FAILURE);
    }

    if(sqlite3_open("system.db",&pdb) != SQLITE_OK)
    {
        fprintf(stderr,"sqlite3 open %s : %s.\n",argv[3],sqlite3_errmsg(pdb));
        exit(EXIT_FAILURE);
    }
    
    while (1)
    {
        struct sockaddr_in clientAddr = {0};
        int len = sizeof(clientAddr);
        new_fd = accept(sockfd, (struct sockaddr*)&clientAddr, &len); 
        printf("new_fd = %d\n", new_fd);
        int fd = fork();
        if (fd == 0)
        {
            //每个新的客户端的接入，重新创建子进程去进程处理
            close(sockfd);
            do_client(new_fd, pdb);
        }
        close(new_fd);
    }
    return 0;
}





