#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include "head.h"

global_t global;

void signal_handler(int signum)
{
	/*
	 *关闭设备 
	 */

	return;
}

void init_global(global_t *pglobal)
{
	pglobal->stop = 0;
	pglobal->length = HEIGHT * WIDTH << 1;

#if 1
	if((pglobal->start = malloc(pglobal->length)) == NULL)
	{
		perror("Fail to malloc");
		exit(EXIT_FAILURE);
	}
#endif 

	if(pthread_mutex_init(&(pglobal->update_lock),NULL) < 0)
	{
		perror("Fail to pthread_mutex_init");
		exit(EXIT_FAILURE);
	}

	if(pthread_cond_init(&(pglobal->update_cond),NULL) < 0)
	{
		perror("Fail to pthread_cond_init");
		exit(EXIT_FAILURE);
	}

	if(pthread_mutex_init(&(pglobal->open_lock),NULL) < 0)
	{
		perror("Fail to pthread_mutex_init");
		exit(EXIT_FAILURE);
	}

	if(pthread_cond_init(&(pglobal->open_cond),NULL) < 0)
	{
		perror("Fail to pthread_cond_init");
		exit(EXIT_FAILURE);
	}

	return;
}

//./a.out port www
int main(int argc,char *argv[])
{
	int ret;
	server_t server; 
	pthread_t cam_tid;
	int cam_fd;

	if(argc < 3)
	{
		fprintf(stderr,"Usage : %s ip port www.\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	//www 参数处理 
	if(*(argv[2] + strlen(argv[2]) - 1) == '/')
		*(argv[2] + strlen(argv[2]) - 1) = '\0';
	server.www_dir = (char *)malloc(strlen(argv[2]));
	strcpy(server.www_dir,argv[2]);
	
	//忽略SIGPIPE信号
	if(signal(SIGPIPE,SIG_IGN) == SIG_ERR)
	{
		perror("Fail to signal");
		exit(EXIT_FAILURE);
	}

	//捕捉ctrl+c信号 
	if(signal(SIGINT,signal_handler) == SIG_ERR)
	{
		perror("Fail to signal");
		exit(EXIT_FAILURE);
	}
	
	//打开视频设备 
	if((cam_fd = open("/dev/video0",O_RDWR | O_NONBLOCK)) < 0)
	{
		perror("Fail to open /dev/video0");
	}

	//初始化视频设备 
	init_camer_device(cam_fd);
	//初始化数据采集方式:mmap 
	video_gather_mmap(cam_fd);

	//初始化global
	init_global(&global);	

	//初始化tcp连接 
	server.listen_fd = init_tcp(argv[1]);
	
	//创建采集视频数据线程 
	ret = pthread_create(&cam_tid,NULL,capture_stream,(void *)&cam_fd);
	if(ret != 0){
		perror("Fail to pthread_create");
		exit(EXIT_FAILURE);
	}

	//创建接收客户端请求线程 
	ret = pthread_create(&server.tid,NULL,accept_client_request,(void *)&server);
	if(ret != 0){
		perror("Fail to pthread_create");
		exit(EXIT_FAILURE);
	}

	//等待 
	pause();
	
	exit(EXIT_SUCCESS);
}
