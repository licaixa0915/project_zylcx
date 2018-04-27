#pragma once
#include <qslider.h>
#include <QMouseEvent>

class VideoSlider:public QSlider
{
	Q_OBJECT
public:
	VideoSlider(QWidget *p);
	~VideoSlider();
	void mousePressEvent(QMouseEvent *e);
};

