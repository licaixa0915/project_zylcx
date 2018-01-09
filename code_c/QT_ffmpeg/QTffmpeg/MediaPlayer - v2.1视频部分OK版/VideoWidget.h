#pragma once
#include <QtWidgets/qwidget.h>
#include <QOpenGLWidget>
class VideoWidget :public QOpenGLWidget
{
public:
	VideoWidget(QWidget *p);
	void paintEvent(QPaintEvent *e);//重载基类的函数
	void timerEvent(QTimerEvent *e);//重载基类的函数
	~VideoWidget();
};

