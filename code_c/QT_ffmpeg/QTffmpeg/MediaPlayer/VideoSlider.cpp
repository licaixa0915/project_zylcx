#include "VideoSlider.h"


VideoSlider::VideoSlider(QWidget *p) :QSlider(p)
{
}


VideoSlider::~VideoSlider()
{
}

void VideoSlider::mousePressEvent(QMouseEvent *e)
{
	int value = (float)e->pos().x() / (float)this->width()*(this->maximum() + 1);
	printf("mousePressEvent:x = %d, width = %d, max = %d\n", e->pos().x(), this->width(), this->maximum() + 1);
	this->setValue(value);
	QSlider::mousePressEvent(e);
	
	return;
}
