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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "videowidget.h"

QT_BEGIN_NAMESPACE

class Ui_MediaPlayerClass
{
public:
    QWidget *centralWidget;
    VideoWidget *openGLWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MediaPlayerClass)
    {
        if (MediaPlayerClass->objectName().isEmpty())
            MediaPlayerClass->setObjectName(QStringLiteral("MediaPlayerClass"));
        MediaPlayerClass->resize(800, 600);
        QIcon icon;
        icon.addFile(QStringLiteral(":/MediaPlayer/Resources/logo.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MediaPlayerClass->setWindowIcon(icon);
        centralWidget = new QWidget(MediaPlayerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        openGLWidget = new VideoWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(-10, -50, 800, 600));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(279, 410, 61, 46));
        pushButton->setStyleSheet(QLatin1String("QPushButton:!hover{border-image: url(:/MediaPlayer/Resources/open_normal.png);}\n"
"QPushButton:hover{border-image: url(:/MediaPlayer/Resources/open_hot.png);}"));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(469, 410, 61, 46));
        pushButton_2->setStyleSheet(QLatin1String("QPushButton:!hover{border-image: url(:/MediaPlayer/Resources/play_normal.png);}\n"
"QPushButton:hover{border-image: url(:/MediaPlayer/Resources/play_hot.png);}"));
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
        pushButton->setText(QString());
        pushButton_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MediaPlayerClass: public Ui_MediaPlayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDIAPLAYER_H
