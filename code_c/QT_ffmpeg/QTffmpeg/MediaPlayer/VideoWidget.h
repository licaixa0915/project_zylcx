#pragma once
#include <QtWidgets/qwidget.h>
#include <QOpenGLWidget>
class VideoWidget :public QOpenGLWidget
{
public:
	VideoWidget(QWidget *p);
	void paintEvent(QPaintEvent *e);//���ػ���ĺ���
	void timerEvent(QTimerEvent *e);//���ػ���ĺ���
	~VideoWidget();
};

