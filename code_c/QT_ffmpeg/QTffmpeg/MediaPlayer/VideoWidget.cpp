#include "VideoWidget.h"
#include <QPainter>
#include <XFFmpeg.h>
#include "VideoTread.h"

VideoWidget::VideoWidget(QWidget *p) :QOpenGLWidget(p)
{
	/* ����Ƶ */
	//XFFmpeg::Get()->Open("video.mp4");//������Ƶ
	//XFFmpeg::Get()->Open("rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov");//rtsp���紮���Ĳ���

	startTimer(20);
	VideoTread::Get()->start();//�ڴ�����Ļ���ص�ʱ������̵߳�����
	
}
extern bool isExit;
VideoWidget::~VideoWidget()
{
	isExit = true;
}

void VideoWidget::paintEvent(QPaintEvent *e)
{
	if (0 == XFFmpeg::Get()->fileOpenFlag)
	{
		return;
	}
	static int w = 0;
	static int h = 0;
	static QImage* image = NULL;

	if ((w != width()) || (h != height()))
	{
		if (image)
		{
			delete image->bits();
			delete image;
			image = NULL;
		}
	}
	
	if (image == NULL)
	{
		uchar *buf = new uchar[width()*height()*4];
		image = new QImage(buf, width(), height(), QImage::Format_ARGB32);
	}
	w = width();
	h = height();

	QPainter painter;
	painter.begin(this);
#if 0
	AVPacket pkt = XFFmpeg::Get()->Read();
	if (pkt.stream_index != XFFmpeg::Get()->videoStream)//����Ƶ��
	{
		av_packet_unref(&pkt);
		return;
	}
	if (pkt.size == 0)
	{
		return;
	}
	
	AVFrame *yuv = XFFmpeg::Get()->Decode(&pkt);
	av_packet_unref(&pkt);
	if (yuv == NULL)
	{
		return;
	}
#endif
	XFFmpeg::Get()->ToRGB((char*)image->bits(), width(), height());
	painter.drawImage(QPoint(0, 0), *image);

	painter.end();
}

void VideoWidget::timerEvent(QTimerEvent *e)
{
	this->update();
}