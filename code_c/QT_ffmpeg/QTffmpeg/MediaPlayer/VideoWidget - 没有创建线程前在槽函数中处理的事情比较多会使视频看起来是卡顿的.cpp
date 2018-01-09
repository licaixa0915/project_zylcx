#include "VideoWidget.h"
#include <QPainter>
#include <XFFmpeg.h>
VideoWidget::VideoWidget(QWidget *p) :QOpenGLWidget(p)
{
	/* 打开视频 */
	XFFmpeg::Get()->Open("video.mp4");
	startTimer(20);
}

VideoWidget::~VideoWidget()
{

}

void VideoWidget::paintEvent(QPaintEvent *e)
{
	static QImage* image = NULL;
	if (image == NULL)
	{
		uchar *buf = new uchar[width()*height()*4];
		image = new QImage(buf, width(), height(), QImage::Format_ARGB32);
	}
	QPainter painter;
	painter.begin(this);

	AVPacket pkt = XFFmpeg::Get()->Read();
	if (pkt.stream_index != XFFmpeg::Get()->videoStream)//非视频流
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
	XFFmpeg::Get()->ToRGB((char*)image->bits(), width(), height());
	painter.drawImage(QPoint(0, 0), *image);

	painter.end();
}

void VideoWidget::timerEvent(QTimerEvent *e)
{
	this->update();
}