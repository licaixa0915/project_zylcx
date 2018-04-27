#include "VideoTread.h"
#include "XFFmpeg.h"

bool isExit = false;
VideoTread::VideoTread()
{

}


VideoTread::~VideoTread()
{

}

void VideoTread::run()
{
	while (!isExit)
	{
		AVPacket pkt = XFFmpeg::Get()->Read();
		if (pkt.size <= 0)
		{
			msleep(10);
			continue;
		}
		/* ����Ƶ�ļ� */
		if (pkt.stream_index != XFFmpeg::Get()->videoStream)
		{
			av_packet_unref(&pkt);
			continue;
		}
		XFFmpeg::Get()->Decode(&pkt);//����
		av_packet_unref(&pkt);
		if (XFFmpeg::Get()->fps > 0)
		{ 
			msleep(1000 / XFFmpeg::Get()->fps);//��������˽���Ľ���
			//ÿһ֡�ĺ����� 
		}
	}
}
