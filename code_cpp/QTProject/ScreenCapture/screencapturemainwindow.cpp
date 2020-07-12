#include "screencapturemainwindow.h"
#include "ui_screencapturemainwindow.h"
#include <QScreen>
#include <QDateTime>
#include <QDebug>

ScreenCaptureMainWindow::ScreenCaptureMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScreenCaptureMainWindow),
    m_pFloatToolWidget(nullptr),
    m_pCaptureWidget(nullptr)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    connect(ui->btnScreenCapture, SIGNAL(clicked()), this, SLOT(onCaptureClicked()));
    connect(ui->btnFloatTool, SIGNAL(clicked()), this, SLOT(onFloatToolClicked()));
}

void ScreenCaptureMainWindow::onCaptureClicked()
{
    m_pCaptureWidget = new CaptureScreen();
    connect(m_pCaptureWidget, SIGNAL(showGrabToolWidget(QPixmap, QRect)), this, SLOT(onShowGrabToolWidget(QPixmap, QRect)));
    connect(m_pCaptureWidget, SIGNAL(hideGrabToolWidget()), this, SLOT(onHideGrabToolWidget()));

    m_pCaptureWidget->show();
}

void ScreenCaptureMainWindow::onShowGrabToolWidget(QPixmap catureImage, QRect captureRect)
{
    if (!m_pFloatToolWidget)
        m_pFloatToolWidget = new FloatToolWidget();
    if (!m_pFloatToolWidget)
        return;
    m_pFloatToolWidget->move(QPoint(captureRect.bottomLeft().x(), captureRect.bottomLeft().y() + 10));
    m_pFloatToolWidget->setWindowState(Qt::WindowNoState|Qt::WindowActive);
    m_pFloatToolWidget->show();
    m_pFloatToolWidget->activateWindow();
    m_pFloatToolWidget->setCaptureInfo(catureImage, captureRect);
}

void ScreenCaptureMainWindow::onHideGrabToolWidget()
{
    if (m_pFloatToolWidget)
        m_pFloatToolWidget->hide();
}

void ScreenCaptureMainWindow::onFloatToolClicked()
{
    m_pFloatToolWidget = new FloatToolWidget(this);
    m_pFloatToolWidget->show();
}

ScreenCaptureMainWindow::~ScreenCaptureMainWindow()
{
    if (m_pCaptureWidget)
        delete m_pCaptureWidget;
    delete ui;
}
