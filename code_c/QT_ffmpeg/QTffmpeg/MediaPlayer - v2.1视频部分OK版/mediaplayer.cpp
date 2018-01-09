#include "mediaplayer.h"
#include <XFFmpeg.h>
#include <QFileDialog>
#include <QMessageBox>

static bool playSliderPressFlag = false;
static bool isPlay = true;

#define PLAY "QPushButton:!hover{border-image: url(:/MediaPlayer/Resources/play_normal.png);}\
QPushButton:hover{border-image:url(:/MediaPlayer/Resources/play_hot.png); }"

#define PAUSE "QPushButton:!hover{border-image: url(:/MediaPlayer/Resources/pause_normal.png);}\
QPushButton:hover{ border-image:url(:/MediaPlayer/Resources/pause_hot.png); }"

MediaPlayer::MediaPlayer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.getRtsp, SIGNAL(clicked()), this, SLOT(Button_getRtsp_clicked()));
	connect(ui.openLocal, SIGNAL(clicked()), this, SLOT(Button_openLocal_clicked()));
	ui.lineEdit->setText("rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov");
	startTimer(20);
	ui.totalTime->setText("00:00");
	connect(ui.playSlider, SIGNAL(sliderPressed()), this, SLOT(playSliderPress()));
	connect(ui.playSlider, SIGNAL(sliderReleased()), this, SLOT(playSliderRelease()));

	connect(ui.pauseCon, SIGNAL(clicked()), this, SLOT(Button_pauseCon_clicked()));
	Button_pauseCon_clicked();
}

void MediaPlayer::Button_pauseCon_clicked()
{
	printf("1:isPlay = %d\n", isPlay);
	isPlay = !isPlay;
	printf("2:isPlay = %d\n", isPlay);
	XFFmpeg::Get()->isPlay = isPlay;
	if (isPlay)
	{
		ui.pauseCon->setStyleSheet(PLAY);
	}
	else
	{
		ui.pauseCon->setStyleSheet(PAUSE);
	}
	return;
}

void MediaPlayer::playSliderPress()
{
	playSliderPressFlag = true;
}

void MediaPlayer::playSliderRelease()
{
	float pos = 0;
	pos = (float)ui.playSlider->value() / (float)(ui.playSlider->maximum() + 1);
	XFFmpeg::Get()->Seek(pos);
	playSliderPressFlag = false;

}

MediaPlayer::~MediaPlayer()
{

}
void MediaPlayer::resizeEvent(QResizeEvent *e)
{
	ui.openGLWidget->resize(size());//将视频窗口的大小调整成当前窗口的大小
	/* 将窗口中对应的控件进行相应的移动 */
	ui.playSlider->resize(this->width() - 100, ui.playSlider->height());
	ui.playSlider->move(50, this->height() - 200);
	ui.pauseCon->move(this->width() / 2 + 100, this->height() - 150);
	ui.openLocal->move(this->width() / 2 - 100, this->height() - 150);
	ui.playTime->move(50, this->height() - 150);
	ui.totalTime->move(150, this->height() - 150);
}

void MediaPlayer::timerEvent(QTimerEvent *e)
{
	char buf[32] = { 0 };
	sprintf(buf, "%02d:%02d/", XFFmpeg::Get()->pts / 1000 / 60, XFFmpeg::Get()->pts / 1000 % 60);
	ui.playTime->setText(buf);
	if (XFFmpeg::Get()->totalMs)
	{ 
		if (!playSliderPressFlag)
			ui.playSlider->setValue((float)XFFmpeg::Get()->pts / (float)XFFmpeg::Get()->totalMs * (float)1000);
	}

	return;
}

void MediaPlayer::Button_getRtsp_clicked()
{
	int totalMs = XFFmpeg::Get()->Open(ui.lineEdit->text().toLocal8Bit());
	if (totalMs <= 0)
	{
		QMessageBox::information(this, "err", "file open failed!");
		return;
	}
	char buf[32] = { 0 };
	sprintf(buf, "%02d:%02d", totalMs / 1000 / 60, totalMs / 100 % 60);
	ui.totalTime->setText(buf);
	isPlay = true;
	XFFmpeg::Get()->isPlay = isPlay;
	ui.pauseCon->setStyleSheet(PLAY);
	return;
	
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
	char buf[32] = { 0 };
	sprintf(buf, "%02d:%02d", totalMs / 1000 / 60, totalMs / 100 % 60);
	ui.totalTime->setText(buf);
	isPlay = true;
	XFFmpeg::Get()->isPlay = isPlay;
	ui.pauseCon->setStyleSheet(PLAY);
	return;
}