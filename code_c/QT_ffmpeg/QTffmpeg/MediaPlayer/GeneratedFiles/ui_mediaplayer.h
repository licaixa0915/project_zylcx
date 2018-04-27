/********************************************************************************
** Form generated from reading UI file 'mediaplayer.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDIAPLAYER_H
#define UI_MEDIAPLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <videoslider.h>
#include "videowidget.h"

QT_BEGIN_NAMESPACE

class Ui_MediaPlayerClass
{
public:
    QWidget *centralWidget;
    VideoWidget *openGLWidget;
    QPushButton *openLocal;
    QPushButton *pauseCon;
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *getRtsp;
    QLabel *totalTime;
    QLabel *playTime;
    VideoSlider *playSlider;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MediaPlayerClass)
    {
        if (MediaPlayerClass->objectName().isEmpty())
            MediaPlayerClass->setObjectName(QStringLiteral("MediaPlayerClass"));
        MediaPlayerClass->resize(800, 700);
        QIcon icon;
        icon.addFile(QStringLiteral(":/MediaPlayer/Resources/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MediaPlayerClass->setWindowIcon(icon);
        centralWidget = new QWidget(MediaPlayerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        openGLWidget = new VideoWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(0, 80, 800, 600));
        openLocal = new QPushButton(centralWidget);
        openLocal->setObjectName(QStringLiteral("openLocal"));
        openLocal->setGeometry(QRect(279, 540, 61, 46));
        openLocal->setStyleSheet(QLatin1String("QPushButton:!hover{border-image: url(:/MediaPlayer/Resources/open_normal.png);}\n"
"QPushButton:hover{border-image: url(:/MediaPlayer/Resources/open_hot.png);}"));
        pauseCon = new QPushButton(centralWidget);
        pauseCon->setObjectName(QStringLiteral("pauseCon"));
        pauseCon->setGeometry(QRect(469, 540, 61, 46));
        pauseCon->setStyleSheet(QLatin1String("QPushButton:!hover{border-image: url(:/MediaPlayer/Resources/play_normal.png);}\n"
"QPushButton:hover{border-image: url(:/MediaPlayer/Resources/play_hot.png);}"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(160, 20, 491, 41));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 141, 24));
        getRtsp = new QPushButton(centralWidget);
        getRtsp->setObjectName(QStringLiteral("getRtsp"));
        getRtsp->setGeometry(QRect(689, 20, 111, 46));
        getRtsp->setStyleSheet(QStringLiteral("border-image: url(:/MediaPlayer/Resources/get.png);"));
        totalTime = new QLabel(centralWidget);
        totalTime->setObjectName(QStringLiteral("totalTime"));
        totalTime->setGeometry(QRect(170, 550, 108, 21));
        totalTime->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"font: 12pt \"Arial\";"));
        playTime = new QLabel(centralWidget);
        playTime->setObjectName(QStringLiteral("playTime"));
        playTime->setGeometry(QRect(80, 550, 91, 24));
        playTime->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"font: 12pt \"Arial\";"));
        playSlider = new VideoSlider(centralWidget);
        playSlider->setObjectName(QStringLiteral("playSlider"));
        playSlider->setGeometry(QRect(50, 510, 711, 22));
        playSlider->setMaximum(999);
        playSlider->setOrientation(Qt::Horizontal);
        MediaPlayerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MediaPlayerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 37));
        MediaPlayerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MediaPlayerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MediaPlayerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MediaPlayerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MediaPlayerClass->setStatusBar(statusBar);

        retranslateUi(MediaPlayerClass);

        QMetaObject::connectSlotsByName(MediaPlayerClass);
    } // setupUi

    void retranslateUi(QMainWindow *MediaPlayerClass)
    {
        MediaPlayerClass->setWindowTitle(QApplication::translate("MediaPlayerClass", "MediaPlayer", Q_NULLPTR));
        openLocal->setText(QString());
        pauseCon->setText(QString());
        label->setText(QApplication::translate("MediaPlayerClass", "<html><head/><body><p>Rtsp\346\222\255\346\224\276\345\234\260\345\235\200\357\274\232</p></body></html>", Q_NULLPTR));
        getRtsp->setText(QString());
        totalTime->setText(QApplication::translate("MediaPlayerClass", "00:00", Q_NULLPTR));
        playTime->setText(QApplication::translate("MediaPlayerClass", "00:00/", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MediaPlayerClass: public Ui_MediaPlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDIAPLAYER_H
