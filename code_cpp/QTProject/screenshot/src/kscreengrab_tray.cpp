#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QTextEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QGuiApplication>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include "kscreengrabdialog.h"
#include "kscreengrab_tray.h"
#define TRIANGLE_HEIGHT 8
#define MAINTRAYWINDOW_WIDTH 250

KScreenGrabTrayMainWindow::KScreenGrabTrayMainWindow()
    : QWidget(nullptr)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);

    initUi();
    qApp->installEventFilter(this);
}

KScreenGrabTrayMainWindow::~KScreenGrabTrayMainWindow()
{

}

void KScreenGrabTrayMainWindow::initUi()
{
    setFixedSize(MAINTRAYWINDOW_WIDTH, 90);
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QToolButton *pScreenGrab = new QToolButton(this);
    pScreenGrab->setText("截屏");
    pScreenGrab->setIcon(QIcon(QString("://icons_png/grab_screen.png")));
    pScreenGrab->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    connect(pScreenGrab, &QToolButton::clicked, this, &KScreenGrabTrayMainWindow::onScreenShotClick);
    mainLayout->addWidget(pScreenGrab);

    QToolButton *pScreenRecord = new QToolButton(this);
    pScreenRecord->setText("录屏");
    pScreenRecord->setIcon(QIcon(QString("://icons_png/screenshot-two.png")));
    pScreenRecord->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mainLayout->addWidget(pScreenRecord);

    QToolButton *pOcr = new QToolButton(this);
    pOcr->setText("文字识别");
    pOcr->setIcon(QIcon(QString("://icons_png/text-recognition.png")));
    pOcr->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mainLayout->addWidget(pOcr);

    this->setLayout(mainLayout);
}

void KScreenGrabTrayMainWindow::onScreenShotClick()
{
    this->hide();
    QTimer::singleShot(100, [=]()
	{
        KScreenGrabDialog dialog;
        dialog.exec();
    });
}

bool KScreenGrabTrayMainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::ApplicationDeactivate) {
        if (qApp == obj)
        {
            this->hide();
        }
    }
    return QWidget::eventFilter(obj, event);
}

void KScreenGrabTrayMainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QRect rc(rect());

    QColor colorLine = QColor(0, 255, 0);
    QPalette pal;
    pal.setColor(QPalette::Window, colorLine);
    this->setPalette(pal);

    QPainterPath drawPath;// = KDrawHelper::getBoundPath(rc, 4, KDrawHelper::ROUND_ALL);
    QPainterPath drawRectPath;

    // 小三角区域;
	QPolygon trianglePolygon;
    int borderRadius = 4 * qGuiApp->devicePixelRatio();
    qreal radius = 6;
    qreal diameter = 2.0 * radius;
    {
        drawPath.moveTo(rc.x() + 2 + radius, rc.y() + TRIANGLE_HEIGHT);//矩形左上角
        drawPath.lineTo(QPoint(rc.x() + rc.width() / 2 - TRIANGLE_HEIGHT, rc.y() + TRIANGLE_HEIGHT));//三角形左边的点
        drawPath.lineTo(QPoint(rc.x() + rc.width() / 2, rc.y()));//三角形上边的点
        drawPath.lineTo(QPoint(rc.x() + rc.width() / 2 + TRIANGLE_HEIGHT, rc.y() + TRIANGLE_HEIGHT));//三角形右边的点
        drawPath.lineTo(rc.x() - 2 + rc.width() - radius, rc.y() + TRIANGLE_HEIGHT);//矩形右边的点
        drawPath.arcTo(QRectF(rc.x() - 2 + rc.width() - diameter, rc.y() + TRIANGLE_HEIGHT, diameter, diameter), 90, -90);
        drawPath.lineTo(rc.x() - 2 + rc.width(), rc.y() + rc.height() - 2 - radius);//矩形右下角
        drawPath.arcTo(QRectF(rc.x() - 2 + rc.width() - diameter, rc.y() + rc.height() - 2 - diameter, diameter, diameter), 0, -90);
        drawPath.lineTo(rc.x() + 2 + radius, rc.y() + rc.height() - 2);//矩形左下角
        drawPath.arcTo(QRectF(rc.x() + 2, rc.y() + rc.height() - 2 - diameter, diameter, diameter), 270, -90);
        drawPath.lineTo(QPoint(rc.x() + 2, rc.y() +  + TRIANGLE_HEIGHT + radius));//矩形左上角
        drawPath.arcTo(QRectF(rc.x() + 2, rc.y() +  + TRIANGLE_HEIGHT, diameter, diameter), 180, -90);
    }
    // Rect + Triangle;
    QColor color = QColor(238, 238, 238);
    QPen pen = painter.pen();
    QColor penColor = QColor(238, 238, 238);
    pen.setColor(penColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(color);
    
    drawPath.addPolygon(trianglePolygon);
    drawPath.closeSubpath();
    painter.drawPath(drawPath);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // return QWidget::paintEvent(event);
}

KScreenGrabTrayManager::KScreenGrabTrayManager()
{
	initTray();
}

KScreenGrabTrayManager::~KScreenGrabTrayManager()
{
    if (m_pSysTray)
        delete m_pSysTray;
    m_pSysTray = nullptr;
}

void KScreenGrabTrayManager::initTray()
{
	m_pSysTray = new QSystemTrayIcon();
    m_pSysTray->setIcon(QIcon(QString("://icons_png/moneybox-48.png")));            //set icon
    m_pSysTray->setToolTip("截屏、文字识别、屏幕录制功能");//set tip
	m_pSysTray->setVisible(true);

    connect(m_pSysTray, &QSystemTrayIcon::activated, this, &KScreenGrabTrayManager::onTrayIconActivated);
 
    m_pSysTray->show();
    if (!m_pMainWidget)        
        m_pMainWidget = new KScreenGrabTrayMainWindow();
    QTimer::singleShot(500, [=]()
	{
        QPoint pos = m_pSysTray->geometry().center();
        pos = pos + QPoint(-(MAINTRAYWINDOW_WIDTH/2), 0.5f * m_pSysTray->geometry().size().height());
        m_pMainWidget->move(pos);
        m_pMainWidget->show();
	});
}

void KScreenGrabTrayManager::showTrayMainWindow()
{
    if (!m_pMainWidget->isVisible())
    {
        QPoint pos = m_pSysTray->geometry().center();
        pos = pos + QPoint(-(MAINTRAYWINDOW_WIDTH/2), 0.5f * m_pSysTray->geometry().size().height());
        m_pMainWidget->move(pos);
        m_pMainWidget->show();
    }   
    else
    {
        m_pMainWidget->hide();
    }
}

void KScreenGrabTrayManager::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
       showTrayMainWindow();
    }
}


