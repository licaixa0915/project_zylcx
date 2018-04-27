#include "mediaplayer.h"
#include <XFFmpeg.h>
#include <QFileDialog>
#include <QMessageBox>

MediaPlayer::MediaPlayer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.getRtsp, SIGNAL(clicked()), this, SLOT(Button_getRtsp_clicked()));
	connect(ui.openLocal, SIGNAL(clicked()), this, SLOT(Button_openLocal_clicked()));
}

MediaPlayer::~MediaPlayer()
{

}

void MediaPlayer::Button_getRtsp_clicked()
{
	XFFmpeg::Get()->Open(ui.lineEdit->text().toLocal8Bit());
}

void MediaPlayer::Button_openLocal_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Please select video file!");
	if (fileName.isEmpty())
	{
		return;
	}

	this->setWindowTitle(fileName);
	int totalMs = XFFmpeg::Get()->Open(fileName.toLocal8Bit());
	if (totalMs <= 0)
	{
		QMessageBox::information(this, "err", "file open failed!");
		return;
	}
	return;
}