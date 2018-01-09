#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QtWidgets/QMainWindow>
#include "ui_mediaplayer.h"

class MediaPlayer : public QMainWindow
{
	Q_OBJECT

public:
	MediaPlayer(QWidget *parent = 0);
	~MediaPlayer();
	void timerEvent(QTimerEvent *e);
	void resizeEvent(QResizeEvent *e);//重构基函数的窗口变化函数
private:
	Ui::MediaPlayerClass ui;

public slots:
	void Button_getRtsp_clicked();
	void Button_openLocal_clicked();
	void playSliderPress();
	void playSliderRelease();
	void Button_pauseCon_clicked();
};

#endif // MEDIAPLAYER_H
