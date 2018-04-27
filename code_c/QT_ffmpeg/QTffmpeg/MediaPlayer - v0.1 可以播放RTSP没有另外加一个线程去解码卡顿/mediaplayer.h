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

private:
	Ui::MediaPlayerClass ui;
};

#endif // MEDIAPLAYER_H
