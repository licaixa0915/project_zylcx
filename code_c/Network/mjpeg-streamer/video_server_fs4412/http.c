#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "head.h"

int init_tcp(char *port)
{
	int on = 1;
	int listen_fd;
	int addr_len = sizeof(struct sockaddr);
	struct sockaddr_in server_addr,peer_addr;

	if((listen_fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("Fail to socket");
		exit(EXIT_FAILURE);
	}

	if(setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
	{
		perror("Fail to setsockopt");
		exit(EXIT_FAILURE);
	}

	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(atoi(port));
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listen_fd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
	{
		perror("Fail to bind");
		exit(EXIT_SUCCESS);
	}

	listen(listen_fd,5);
	printf("Listen ...\n");

	return listen_fd;
}

int accept_http_request(int sockfd,char *buf)
{
	fd_set fds;
	int ret;
	int n = 0,offset = 0;
	struct timeval tv = {5,0};

	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(sockfd,&fds);

		if((ret = select(sockfd + 1,&fds,NULL,NULL,&tv)) <= 0)
		{
			if(ret < 0){
				perror("Fail to select");
				return -1;
			}

			break;
		}
		
		if((n = recv(sockfd,buf + offset,BUFFER_SIZE - offset,0)) < 0)
		{
			perror("Fail to recv");
			return -1;
		}
#if 0			
		printf("***********************************\n");
		printf("%s\n",buf);
		printf("***********************************\n");
#endif 	
		offset += n;

		if(offset >= sizeof(buf) - 1)
			break;
	}
	
	return 0;
}

int analysis_http_request(char *buf,request_t *req)
{
	int len = 0;
	char *p = NULL;
	
	/* determine what to deliver */
	if ( strstr(buf, "GET /?action=snapshot") != NULL ) {
			req->type = A_SNAPSHOT;
			return 0;
	}else if ( strstr(buf, "GET /?action=stream") != NULL ) {
			req->type = A_STREAM;
			return 0;
	}else if(strstr(buf,"GET /open") != NULL){
			printf("send signal!\n");
			pthread_mutex_lock(&global.open_lock);
			pthread_cond_signal(&global.open_cond);
			pthread_mutex_unlock(&global.open_lock);
			return 0;
	}else if(strstr(buf,"GET /close") != NULL){
			global.stop = 1;
			return 0;
	}else{

		if((p = strstr(buf,"GET /")) == NULL)
		{
			printf("Http request error.\n");
			//send error to client
			return -1;
		}

		p += strlen("GET /");
		len = MIN(MAX(strspn(p, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._-1234567890"), 0), 100);
		req->parm = (char *)malloc(len + 1);
		memset(req->parm,0,len+1);
		memcpy(req->parm,p,len);
	//	printf("req->parm : %s.\n",req->parm);
		req->type = A_FILE;
	}

	return 0;
}

void send_snapshot(int sockfd)
{
	int length;
	char *frame;
	char buf[BUFFER_SIZE];

//	pthread_mutex_lock(&global.update_lock);

	pthread_cond_wait(&global.update_cond,&global.update_lock);	
	
	//获得视频数据
	length = global.length;
	frame  = (char *)malloc(global.length);
	memcpy(frame,global.start,global.length);
	
	pthread_mutex_unlock(&global.update_lock);

	//添加http头	
    memset(buf,0,sizeof(buf));
	sprintf(buf, "HTTP/1.0 200 OK\r\n" \
                 "Content-type: image/jpeg\r\n" \
                 STD_HEADER \
         		 "\r\n");

	//发送http头
	if(send(sockfd,buf,strlen(buf),0) < 0)
	{
		free(frame);
		return;
	}

	//发送视频数据  
	if(send(sockfd,frame,length,0) < 0)
	{
		free(frame);
		return;
	}

	free(frame);

	return;
}

void send_stream(int sockfd)
{
	int length;
	char *frame;
	char buf[BUFFER_SIZE];
	
	printf("send_stream\n");

	sprintf(buf, "HTTP/1.1 200 OK\r\n" \
			STD_HEADER \
			"Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n" \
			"\r\n" \
			"--" BOUNDARY "\r\n");

	if(send(sockfd,buf,strlen(buf),0) < 0)
	{
		perror("fail to send http head");
		return;
	}

	printf("send http head ok (global.stop : %d\n) .............\n",global.stop);
	
	printf("Waiting open ...\n");
	pthread_cond_wait(&global.open_cond,&global.open_lock);
	pthread_mutex_unlock(&global.open_lock);

	while(!global.stop)
	{
		printf("Waiting ....\n");
	//	pthread_mutex_lock(&global.update_lock);
		pthread_cond_wait(&global.update_cond,&global.update_lock);
		printf("wake ok!.....................................\n");			
		//获得视频数据
		length = global.length;
		frame  = (char *)malloc(global.length);
		memcpy(frame,global.start,global.length);
		
		pthread_mutex_unlock(&global.update_lock);
	
		/* print the individual mimetype and the length
		 * sending the content-length fixes random stream disruption observed
		 * with firefox
		 */
		sprintf(buf, "Content-Type: image/jpeg\r\n" \
				"Content-Length: %d\r\n" \
				"\r\n",length);
		//sending intermdiate header
		if(send(sockfd,buf,strlen(buf),0) < 0)
		{
			perror("Fail to send intermdiate header");
			return;
		}

		printf("send intermdiate header.\n");
		
		//sending camera frame 
		if(send(sockfd,frame,length,0) < 0)
		{
			perror("Fail to send camera frame");
			return;
		}

		sprintf(buf, "\r\n--" BOUNDARY "\r\n");
		if ( send(sockfd, buf, strlen(buf),0) < 0 ) break;
	
		free(frame);
	}

	return;
}

void send_file(int sockfd,char *pathfile,char *www_dir)
{
	int n,fd,i;
	char buf[1024] = {0};
    char *extension, *mimetype=NULL;
    
	if(pathfile == NULL || strlen(pathfile) == 0)
		pathfile = "index.html";
	
    /* find file-extension */
    if ( (extension = strstr(pathfile, ".")) == NULL ) {
    //    send_error(fd, 400, "No file extension found");
        return;
    }

    /* determine mime-type */
    for ( i=0; i < LENGTH_OF(mimetypes); i++ ) {
        if ( strcmp(mimetypes[i].dot_extension, extension) == 0 ) {
            mimetype = (char *)mimetypes[i].mimetype;
            break;
        }
    }
    
    /* in case of unknown mimetype or extension leave */
    if ( mimetype == NULL ) {
    //    send_error(fd, 404, "MIME-TYPE not known");
        return;
    }

	//打开文件 
	sprintf(buf,"%s/%s",www_dir,pathfile);
	if((fd = open(buf,O_RDONLY)) < 0)
	{
		fprintf(stderr,"Fail to open %s : %s.\n",buf,strerror(errno));
		//send error to webbrowser
		return;
	}

//	printf("pathfile : %s.\n",buf);

	//添加http头	
    memset(buf,0,sizeof(buf));
	sprintf(buf, "HTTP/1.0 200 OK\r\n" \
                 "Content-type: %s\r\n" \
                 STD_HEADER \
                 "\r\n",mimetype);
	
    n = strlen(buf);
	
	//发送http头信息和网页
	do{
		send(sockfd,buf,n,0);
	}while(n = read(fd,buf,sizeof(buf)));

	return;
}

void *client_thread(void *arg)
{
	request_t request = {0};
	char buf[1024] = {0};
	client_t *pclient = (client_t *)arg;
		
	//接受浏览器发过来的请求
	if(recv(pclient->connect_fd,buf,sizeof(buf) -1,0) < 0)
	{
		perror("Fail to recv");
		pthread_exit(NULL);
	}

	//分析请求
	if(analysis_http_request(buf,&request) < 0){
		pthread_exit(NULL);
	}

	//应答浏览器
	switch(request.type)
	{
	case A_FILE:
		send_file(pclient->connect_fd,request.parm,pclient->www_dir);
		break;

	case A_SNAPSHOT:
		send_snapshot(pclient->connect_fd);
		break;

	case A_STREAM:
		send_stream(pclient->connect_fd);
		break;
	}

	close(pclient->connect_fd);
	free(pclient->www_dir);
	printf("free pclient->www_dir ok.\n");
	free(pclient);
	printf("free pclient ok.\n");
	free(request.parm);
	printf("free request.parm ok.\n");

	return NULL;
}

void *accept_client_request(void *arg)
{
	int ret;
	client_t *pclient;
	struct sockaddr_in peer_addr;	
	server_t *pserver = (server_t *)arg;
	int addr_len = sizeof(struct sockaddr);
	
	/*并发接收客户端发来的请求*/
	while(1)
	{
		pclient = (client_t *)malloc(sizeof(client_t));
		pclient->www_dir = (char *)malloc(strlen(pserver->www_dir));
		strcpy(pclient->www_dir,pserver->www_dir);

		if((pclient->connect_fd = accept(pserver->listen_fd,(struct sockaddr *)&peer_addr,&addr_len)) < 0)
		{
			perror("Fail to accept");
			exit(EXIT_FAILURE);
		}
#if 0
		printf("****************************\n");
		printf("Ip : %s.\n",inet_ntoa(peer_addr.sin_addr));
		printf("Port : %d.\n",ntohs(peer_addr.sin_port));
		printf("****************************\n");
#endif 		
		ret = pthread_create(&(pclient->tid),NULL,client_thread,(void *)pclient); 
		if(ret != 0){
			perror("Fail to pthread_create");
		}

		pthread_detach(pclient->tid);
	}

	return;
}
