#include "mtoolbarheader.h"
#include "screengrab_toolbar.h"
#include <QAbstractButton>
#include "mscreentools.h"
#include <QHelpEvent>
#include <QSettings>
#include <QApplication>

#define __STRINGIFY( _x ) # _x
#define STRINGIFY( _x ) __STRINGIFY( _x )

class Settings : public QSettings
{
public:
    Settings();
    ~Settings();

    static QSettings::Format XmlFormat;

private:
    QString _organizationName() const;
};
QSettings::Format Settings::XmlFormat = QSettings::InvalidFormat;

Settings::Settings()
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, _organizationName(), "Office\\6.0\\plugins")
{

}

Settings::~Settings()
{

}

QString Settings::_organizationName() const
{
    return QApplication::instance()->organizationName();
}


MToolBarHeaderManager::MToolBarHeaderManager(QObject *parent)
    : QObject{parent}
{

}
void MToolBarHeaderManager::init(KScreenGrabDialog *parent, DrawShape drawShape)
{
    if (m_toolbarHeader)
        return;
    m_drawShape = drawShape;

    QColor btnCl = QColor("#FFFFFF");
    QColor btnHover = QColor("#EEEEEE");
    QColor btnDown = QColor("#EEEEEE");
    QColor bgCl = QColor("#FFFFFF");
    QColor clText = QColor("#0D0D0D");

    QString strBtn = QString("rgba(%1, %2, %3, %4)").arg(btnCl.red()).arg(btnCl.green()).arg(btnCl.blue()).arg(btnCl.alpha());
    QString strHover = QString("rgba(%1, %2, %3, %4)").arg(btnHover.red()).arg(btnHover.green()).arg(btnHover.blue()).arg(btnHover.alpha());
    QString strDown = QString("rgba(%1, %2, %3, %4)").arg(btnDown.red()).arg(btnDown.green()).arg(btnDown.blue()).arg(btnDown.alpha());
    QString strbg = QString("rgba(%1, %2, %3, %4)").arg(bgCl.red()).arg(bgCl.green()).arg(bgCl.blue()).arg(bgCl.alpha());
    QString strText = QString("rgba(%1, %2, %3, %4)").arg(clText.red()).arg(clText.green()).arg(clText.blue()).arg(clText.alpha());
    m_toolbarHeader = new KScreenGrabToolBarHeader(parent);
    m_toolbarHeader->setMouseTracking(true);
    m_toolbarHeader->installEventFilter(this);
    m_toolbarHeader->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    QString strStyle = QString("QToolBar{background-color: %1; border:1px solid rgba(0,0,0,38);border-radius:4px;padding:8px;}\
                               QToolButton{margin-left:2px;margin-right:2px;margin-top:0px;margin-bottom:0px;\
                               border-style:flat;border-radius:4px;height:23px;width:8px;\
                               color:%2;font-family:'PingFang SC';font-size:12px; padding-top:8px;}\
                               QToolButton:on{background-color: %3}\
                               QToolButton:hover{background-color: %4}\
                               QToolButton:pressed{background-color: %5}")
                       .arg(strbg)
                       .arg(strText)
                       .arg(strDown)
                       .arg(strHover)
                       .arg(strDown);
    m_toolbarHeader->setStyleSheet(strStyle);

    {
        m_actionDrawRect = new QAction(this);
        m_actionDrawRect->setText(tr("Grab Rect"));
        m_actionDrawRect->setIcon(QIcon(("://icons_svg/other/TopRect.svg")));
        bool b = connect(m_actionDrawRect, SIGNAL(triggered()), this, SLOT(onActionDrawRect()));
        Q_ASSERT(b);
        m_toolbarHeader->addAction(m_actionDrawRect);
        QAbstractButton* button = qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRect));
        if (button)
        {
            button->setFixedSize(QSize(MScreenTools::dpiScaled(100), MScreenTools::dpiScaled(72)));
            button->setCheckable(true);
            button->setCursor(Qt::ArrowCursor);
            button->setProperty("qtspyName", QString("DrawRect"));
            button->installEventFilter(m_toolbarHeader);
            button->setToolTip(QString::fromLocal8Bit(
                STRINGIFY(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"><html><head><meta name="qrichtext" content="1" /><style type="text/css">p { white-space: pre-wrap; } </style></head><body style=" font-family:'PingFang SC'; font-size:12px; font-weight:400; font-style:normal;"><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" color:#4f85ff;">鼠标左键拖动</span><span style="color:#999999;">选取屏幕区域; </span></p><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" color:#4f85ff;">Esc键</span><span style="color:#999999;">可退出截屏</span></p><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'PingFang SC'; font-size:12px; color:#999999;">注: 自动选取全屏或窗口截屏</span></p><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" color:#999999;"></span></p></body></html>
                )
            ));

            m_tooltipSizeHints[button] = QSize(MScreenTools::dpiScaled(206), MScreenTools::dpiScaled(116));
        }
    }
    {
        m_actionDrawEllipse = new QAction(this);
        m_actionDrawEllipse->setText(tr("Grab Ellipse"));
        m_actionDrawEllipse->setIcon(QIcon(("://icons_svg/other/TopEllipse.svg")));
        bool b = connect(m_actionDrawEllipse, SIGNAL(triggered()), this, SLOT(onActionDrawEllipse()));
        Q_ASSERT(b);
        m_toolbarHeader->addAction(m_actionDrawEllipse);
        QAbstractButton* button = qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawEllipse));
        if (button)
        {
            button->setFixedSize(QSize(MScreenTools::dpiScaled(100), MScreenTools::dpiScaled(72)));
            button->setCheckable(true);
            button->setCursor(Qt::ArrowCursor);
            button->setProperty("qtspyName", QString("DrawEllipse"));
            button->installEventFilter(m_toolbarHeader);
                button->setToolTip(QString::fromLocal8Bit(
                    STRINGIFY(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"><html><head><meta name = "qrichtext" content="1"/><style type = "text/css">p { white-space: pre-wrap; } </style></head><body style = " font-family:'PingFang SC'; font-size:12px; font-weight:400; font-style:normal;"><p style = " margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style = " color:#4f85ff;">鼠标左键拖动</span><span style = "color:#999999;">选取屏幕区域; </span></p><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'PingFang SC'; font-size:12px; color:#999999;"> </span></p><p style = " margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style = " color:#4f85ff;">Esc键</span><span style = "color:#999999;">可退出截屏</span></p>
                    )
                ));
            m_tooltipSizeHints[button] = QSize(MScreenTools::dpiScaled(206), MScreenTools::dpiScaled(116));
        }
    }
    {
        m_actionDrawRoundedRect = new QAction(this);
        m_actionDrawRoundedRect->setText(tr("Grab RoundedRect"));
        m_actionDrawRoundedRect->setIcon(QIcon(("://icons_svg/other/TopRoundedRect.svg")));
        bool b = connect(m_actionDrawRoundedRect, SIGNAL(triggered()), this, SLOT(onActionDrawRoundedRect()));
        Q_ASSERT(b);
        m_toolbarHeader->addAction(m_actionDrawRoundedRect);
        QAbstractButton* button = qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRoundedRect));
        if (button)
        {
            button->setFixedSize(QSize(MScreenTools::dpiScaled(100), MScreenTools::dpiScaled(72)));
            button->setCheckable(true);
            button->setCursor(Qt::ArrowCursor);
            button->setProperty("qtspyName", QString("DrawRoundedRect"));
            button->installEventFilter(m_toolbarHeader);
            button->setToolTip(QString::fromLocal8Bit(
                    STRINGIFY(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"><html><head><meta name = "qrichtext" content="1"/><style type = "text/css">p{ white-space: pre-wrap; }</style></head><body style = " font-family:'PingFang SC'; font-size:12px; font-weight:400; font-style:normal;"><p style = " margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style = " color:#4f85ff;">鼠标左键拖动</span><span style = "color:#999999;">选取屏幕区域; </span></p><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'PingFang SC'; font-size:12px; color:#999999;"> </span></p><p style = " margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style = " color:#4f85ff;">Esc键</span><span style = "color:#999999;">可退出截屏</span></p>
                    )
                ));
            m_tooltipSizeHints[button] = QSize(MScreenTools::dpiScaled(206), MScreenTools::dpiScaled(116));
        }
    }

    {
        m_actionDrawPolygon = new QAction(this);
        m_actionDrawPolygon->setText(tr("Grab Polygon"));
        m_actionDrawPolygon->setIcon(QIcon(("://icons_svg/other/TopPolygon.svg")));
        bool b = connect(m_actionDrawPolygon, SIGNAL(triggered()), this, SLOT(onActionDrawPolygon()));
        Q_ASSERT(b);
        m_toolbarHeader->addAction(m_actionDrawPolygon);
        QAbstractButton* button = qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawPolygon));
        if (button)
        {
            button->setFixedSize(QSize(MScreenTools::dpiScaled(100), MScreenTools::dpiScaled(72)));
            button->setCheckable(true);
            button->setCursor(Qt::ArrowCursor);
            button->setProperty("qtspyName", QString("DrawPolygon"));
            button->installEventFilter(m_toolbarHeader);
                // button->setToolTip(QString::fromLocal8Bit(
                // 	STRINGIFY(<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name = \"qrichtext\" content=\"1\"/><style type = \"text/css\">p{ white-space: pre-wrap; }</style></head><body style = \" font-family:'PingFang SC'; font-size:12px; font-weight:400; font-style:normal;\"><p style = \" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style = \" color:#4f85ff;\">Left mouse click </span><span style = \"color:#999999;\">to select drawing points, Get polygon area.</span></p><p style = \" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style = \" color:#4f85ff;\">Left mouse button drag </span><span style = \"color:#999999;\">will record the movement path, Get arbitrary shape area.</span></p><p style = \" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style = \" color:#4f85ff;\">Click the origin </span><span style = \"color:#999999;\">or </span><span style = \" color:#4f85ff;\">double-click with the mouse anywhere </span><span style = \"color:#999999;\">get the current screenshot area.</span></p><p style = \" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style = \" color:#4f85ff;\">Click ESC </span><span style = \"color:#999999;\">to exit editing.</span></p>
                // 	)
                // ));
            m_tooltipSizeHints[button] = QSize(MScreenTools::dpiScaled(320), MScreenTools::dpiScaled(136));
        }
    }
    toggleDrawAction();
    showToolBarHeader();
}
void MToolBarHeaderManager::showToolBarHeader()
{
    if (m_toolbarHeader)
        m_toolbarHeader->show();
}
void MToolBarHeaderManager::hideToolBarHeader()
{
    if (m_toolbarHeader)
        m_toolbarHeader->hide();
}

void MToolBarHeaderManager::toggleDrawAction()
{
    if (m_toolbarHeader)
    {
        QAbstractButton* widgets[] = {
            qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRect)),
            qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRoundedRect)),
            qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawEllipse)),
            qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawPolygon)),
        };

        for (int i = 0; i < sizeof(widgets) / sizeof(QAbstractButton*); ++i)
        {
            if (widgets[i])
                widgets[i]->setChecked(false);
        }

        widgets[m_drawShape]->setChecked(true);
    }

    emit sigToggleDrawAction();
}

void MToolBarHeaderManager::onActionDrawRect()
{
    m_drawShape = dsRect;
    drawShapeChanged(m_drawShape);
    toggleDrawAction();
}
void MToolBarHeaderManager::onActionDrawRoundedRect()
{
    m_drawShape = dsRoundedRect;
    drawShapeChanged(m_drawShape);
    toggleDrawAction();
}

void MToolBarHeaderManager::onActionDrawEllipse()
{
    m_drawShape = dsEllipse;
    drawShapeChanged(m_drawShape);
    toggleDrawAction();
}

void MToolBarHeaderManager::onActionDrawPolygon()
{
    m_drawShape = dsPolygon;
    drawShapeChanged(m_drawShape);
    toggleDrawAction();
}

const QSize* MToolBarHeaderManager::getTooltipSizeHint(QWidget* w)
{
    auto iter = m_tooltipSizeHints.constFind(w);
    if (iter == m_tooltipSizeHints.end())
        return nullptr;
    return &*iter;
}

KScreenGrabToolBarHeader * MToolBarHeaderManager::toolBarHeader()
{
    return m_toolbarHeader;
}

void MToolBarHeaderManager::showToolTip()
{
    const QString STR_GRAB_SCREEN = "GrabScreen";
    const QString STR_GRAB_SCREEN_SHAPE_USED = "GrabScreen_ShapeUsed";
    Settings s;
    s.beginGroup(STR_GRAB_SCREEN);
    int used = s.value(STR_GRAB_SCREEN_SHAPE_USED).toInt();
    if (used & (1 << m_drawShape))
    {
        // 用过这个形状, 什么也不做
    }
    else
    {
        // 显示对应的tooltip
        QAbstractButton* widgets[] = {
            qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRect)),
            qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRoundedRect)),
            qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawEllipse)),
            qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawPolygon)),
        };
        QHelpEvent e(QEvent::Enter, QPoint(), QPoint());
        if (widgets[m_drawShape])
            m_toolbarHeader->eventFilter(widgets[m_drawShape], &e);

        used |= (1 << m_drawShape);
        s.setValue(STR_GRAB_SCREEN_SHAPE_USED, used);
    }
}
