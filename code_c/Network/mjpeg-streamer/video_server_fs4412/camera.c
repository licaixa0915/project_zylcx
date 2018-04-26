#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <linux/videodev2.h>
#include "head.h"

static camera_t *camera_data;
static int n_buffer = 0;

//初始化视频设备
int init_camer_device(int fd)
{
	struct v4l2_fmtdesc fmt;
	struct v4l2_capability cap;
	struct v4l2_format stream_fmt;
	int ret;
	
	//当前视频设备支持的视频格式
	memset(&fmt,0,sizeof(fmt));
	fmt.index = 0;
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

#if 1
	while((ret = ioctl(fd,VIDIOC_ENUM_FMT,&fmt)) == 0)
	{
		fmt.index ++ ;

		printf("{pixelformat = %c%c%c%c},description = '%s'\n",
				fmt.pixelformat & 0xff,(fmt.pixelformat >> 8)&0xff,
				(fmt.pixelformat >> 16) & 0xff,(fmt.pixelformat >> 24)&0xff,
				fmt.description);
	}
#endif 

	//查询视频设备驱动的功能
	ret = ioctl(fd,VIDIOC_QUERYCAP,&cap);
	if(ret < 0){
		perror("FAIL to ioctl VIDIOC_QUERYCAP");
		exit(EXIT_FAILURE);
	}

	//判断是否是一个视频捕捉设备
	if(!(cap.capabilities & V4L2_BUF_TYPE_VIDEO_CAPTURE))
	{
		printf("The Current device is not a video capture device\n");
		exit(EXIT_FAILURE);
	
	}

	//判断是否支持视频流形式
	if(!(cap.capabilities & V4L2_CAP_STREAMING))
	{
		printf("The Current device does not support streaming i/o\n");
		exit(EXIT_FAILURE);
	}

	//设置摄像头采集数据格式，如设置采集数据的
	//长,宽，图像格式(JPEG,YUYV,MJPEG等格式) 
	stream_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	stream_fmt.fmt.pix.width = WIDTH; 
	stream_fmt.fmt.pix.height = HEIGHT;

	stream_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
//	stream_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	stream_fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

	if(-1 == ioctl(fd,VIDIOC_S_FMT,&stream_fmt))
	{
		perror("Fail to ioctl");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}

int video_gather_mmap(int fd)
{
	int i = 0;
	enum v4l2_buf_type type;
	struct v4l2_requestbuffers reqbuf;

	bzero(&reqbuf,sizeof(reqbuf));
	reqbuf.count = 7;
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.memory = V4L2_MEMORY_MMAP;
	
	//申请视频缓冲区(这个缓冲区位于内核空间，需要通过mmap映射)
	//这一步操作可能会修改reqbuf.count的值，修改为实际成功申请缓冲区个数
	if(-1 == ioctl(fd,VIDIOC_REQBUFS,&reqbuf))
	{
		perror("Fail to ioctl 'VIDIOC_REQBUFS'");
		exit(EXIT_FAILURE);
	}
		
	n_buffer = reqbuf.count;

	camera_data = calloc(reqbuf.count,sizeof(*camera_data));
	if(camera_data == NULL){
		fprintf(stderr,"Out of memory\n");
		exit(EXIT_FAILURE);
	}

	//将内核缓冲区映射到用户进程空间
	for(i = 0; i < reqbuf.count; i ++)
	{
		struct v4l2_buffer buf;
		
		bzero(&buf,sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		//查询申请到内核缓冲区的信息
		if(-1 == ioctl(fd,VIDIOC_QUERYBUF,&buf))
		{
			perror("Fail to ioctl : VIDIOC_QUERYBUF");
			exit(EXIT_FAILURE);
		}

		camera_data[i].length = buf.length;
		camera_data[i].start = 
			mmap(
					NULL,/*start anywhere*/
					buf.length,
					PROT_READ | PROT_WRITE,
					MAP_SHARED,
					fd,buf.m.offset
				);
		if(MAP_FAILED == camera_data[i].start)
		{
			perror("Fail to mmap");
			exit(EXIT_FAILURE);
		}
	}	

	//将申请的内核缓冲区放入一个队列中
	for(i = 0;i < reqbuf.count;i ++)
	{
		struct v4l2_buffer buf;

		bzero(&buf,sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		
		if(-1 == ioctl(fd,VIDIOC_QBUF,&buf))
		{	perror("Fail to ioctl 'VIDIOC_QBUF'");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

//将采集好的数据放到文件中
int process_image(void *addr,int length)
{
	FILE *fp;
	static int num = 0;
	char picture_name[20];
	
//	sprintf(picture_name,"test.avi");
	sprintf(picture_name,"picture%d.jpg",num ++);
	
	if((fp = fopen(picture_name,"w")) == NULL)
	{
		perror("Fail to fopen");
		exit(EXIT_FAILURE);
	}

	fwrite(addr, length, 1, fp);
	usleep(500);

	fclose(fp);
	exit(0);
	return 0;
}

int read_frame(int fd)
{
	struct v4l2_buffer buf;
	unsigned int i;

	bzero(&buf,sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	
	//从队列中取缓冲区
	if(-1 == ioctl(fd,VIDIOC_DQBUF,&buf))
	{
		perror("Fail to ioctl 'VIDIOC_DQBUF'");
		exit(EXIT_FAILURE);
	}

	assert(buf.index < n_buffer);
		
	pthread_mutex_lock(&global.update_lock);

#if 0
	//读取到全局的buffer中去
	global.length = camera_data[buf.index].length;
	
	//采集的数据大于预定义的大小,扩充内存
	if(global.length > WIDTH * HEIGHT)
	{
		global.start = realloc(global.start,global.length);
		memcpy(global.start,camera_data[buf.index].start,global.length);
	}else{
		memcpy(global.start,camera_data[buf.index].start,global.length);
	}
#endif 

	//char src[WIDTH * HEIGHT * 3]; 
	//memcpy(src,camera_data[buf.index].start,camera_data[buf.index].length);
	//
	//将YUYV格式的数据专程JPEG格式,将转换好的数据放到global.start,然后由
	//通过http协议发送到浏览器
	global.length = compress_yuyv_to_jpeg(camera_data[buf.index].start,global.start,global.length,80);	
	
//	process_image(global.start,global.length);

	pthread_cond_broadcast(&global.update_cond);
	printf("pthread_cond_broadcast\n");
	pthread_mutex_unlock(&global.update_lock);

	if(-1 == ioctl(fd,VIDIOC_QBUF,&buf))
	{
		perror("Fail to ioctl 'VIDIOC_QBUF'");
		exit(EXIT_FAILURE);
	}

	return 1;
}

void *capture_stream(void *arg)
{
	int r;
	fd_set fds;
	struct timeval tv;
	int fd = *((int *)arg);
	enum v4l2_buf_type type;
	extern void uninit_camer_device();
	extern void close_camer_device();
	int i = 0;

	//开始采集数据
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd,VIDIOC_STREAMON,&type))
	{
		perror("Fail to ioctl 'VIDIOC_STREAMON'");
		exit(EXIT_FAILURE);
	}

	while(!global.stop)
	{
		for(i = 0;i < 7;i ++)
		{   
			//-----------------------------------------	
			FD_ZERO(&fds);
			FD_SET(fd,&fds);

			/*Timeout*/
			tv.tv_sec = 2;
			tv.tv_usec = 0;
		
			r = select(fd + 1,&fds,NULL,NULL,&tv);

			if(-1 == r)
			{
				if(EINTR == errno)
					continue;
				
				perror("Fail to select");
				exit(EXIT_FAILURE);
			}

			if(0 == r)
			{
				fprintf(stderr,"select Timeout\n");
#if 1
				uninit_camer_device();
				close_camer_device(fd);

				//重新视频设备 
				if((fd = open("/dev/video0",O_RDWR | O_NONBLOCK)) < 0)
				{
					perror("Fail to open /dev/video0");
				}
				
				//初始化视频设备 
				init_camer_device(fd);
				//初始化数据采集方式:mmap 
				video_gather_mmap(fd);

				//开始采集数据
				type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
				if(-1 == ioctl(fd,VIDIOC_STREAMON,&type))
				{
					perror("Fail to ioctl 'VIDIOC_STREAMON'");
					exit(EXIT_FAILURE);
				}
#endif 
				break;
			}

//			printf("video is ok.\n");
			if(read_frame(fd))
			{
				continue;
			}
		}
	}

	pthread_exit(NULL);
}

void stop_capturing(int fd)
{
	enum v4l2_buf_type type;
	
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd,VIDIOC_STREAMOFF,&type))
	{
		perror("Fail to ioctl 'VIDIOC_STREAMOFF'");
		exit(EXIT_FAILURE);
	}
	
	return;
}

void uninit_camer_device()
{
	unsigned int i;

	for(i = 0;i < n_buffer;i ++)
	{
		if(-1 == munmap(camera_data[i].start,camera_data[i].length))
		{
			exit(EXIT_FAILURE);
		}
	}
	
	free(camera_data);

	return;
}

void close_camer_device(int fd)
{
	if(-1 == close(fd))
	{
		perror("Fail to close fd");
		exit(EXIT_FAILURE);
	}

	return;
}
