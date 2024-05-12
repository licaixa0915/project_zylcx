#include "mscreengrabtip.h"
#include "kscreengrabdialog.h"
#include "mscreentools.h"
#include "mtoolbarheader.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
// -----------------------------------------------------------------------
#define TRI_Y 8 // 三角形Y坐标

KScreenGrabTip::KScreenGrabTip(KScreenGrabDialog* parent /*= nullptr*/)
    : QWidget(parent)
    , m_main(nullptr)
    , m_label(nullptr)
    , m_dlg(parent)
    , m_title(nullptr)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    // bool b = connect(kcoreApp, SIGNAL(notifyAppSkinChange(QString)), this, SLOT(onChangeSkinMode()));
    // Q_ASSERT(b);
}

void KScreenGrabTip::reset(QWidget* btn)
{
    const QSize* szPtr = m_dlg->toolBarHeaderManager()->getTooltipSizeHint(btn);
    if (szPtr)
        setGeometry(QRect(geometry().topLeft(), *szPtr));

    QRect rc = geometry();
    rc.adjust(MScreenTools::dpiScaled(2), MScreenTools::dpiScaled(TRI_Y), MScreenTools::dpiScaled(-2), MScreenTools::dpiScaled(-2));

    if (!m_main)
    {
        m_main = new QWidget(this);
        m_main->installEventFilter(this);
        QVBoxLayout* vl = new QVBoxLayout(m_main);
        vl->setSpacing(MScreenTools::dpiScaled(8));

        m_title = new QLabel(m_main);
        m_title->setStyleSheet(" background-color: white; color: black");
        QFont font;
        font.setPixelSize(14);
        font.setFamily("PingFang SC");
        m_title->setFont(font);
        m_title->setText(tr("Intro:"));
        vl->addSpacing(MScreenTools::dpiScaled(6));
        vl->addWidget(m_title);

        m_label = new QLabel(m_main);
        vl->addWidget(m_label);
        vl->addStretch();
    }

    if (!m_main->graphicsEffect())
    {
        QGraphicsDropShadowEffect* eff = new QGraphicsDropShadowEffect();
        eff->setOffset(0);
        eff->setBlurRadius(MScreenTools::dpiScaled(8));
        m_main->setGraphicsEffect(eff);
    }

    m_main->setGeometry(QRect(QPoint(MScreenTools::dpiScaled(2), MScreenTools::dpiScaled(TRI_Y)), rc.size()));
    m_main->setGeometry(QRect(QPoint(0, 0), rc.size()));
    m_label->setAlignment(Qt::AlignTop);
    m_label->setText(m_text);
    onChangeSkinMode();
}

bool KScreenGrabTip::eventFilter(QObject * o, QEvent * e)
{
    if (o == m_main)
    {
        QWidget* w = qobject_cast<QWidget*>(o);
        if (w)
        {
            paintEvent(w, static_cast<QPaintEvent*>(e));
            return true;
        }
    }
    return QWidget::eventFilter(o, e);
}

void KScreenGrabTip::onChangeSkinMode()
{
    // QColor cl = KDrawHelper::getColorFromTheme("KScreenGrabTip", KDrawHelper::Prop_Text);
    QColor cl = QColor(Qt::black);
    QString strText = QString("rgba(%1, %2, %3, %4)").arg(cl.red()).arg(cl.green()).arg(cl.blue()).arg(cl.alpha());
    if (m_title)
        m_title->setStyleSheet(QString("QLabel{background-color: transparent;font-family: 'PingFang SC' ;color: %1;font-size:14px;}").arg(strText));

    if (m_label)
        m_label->setStyleSheet(QString("QLabel{background-color: transparent;font-family: 'PingFang SC' ;color: %1;font-size:12px;}").arg(strText));
}

void KScreenGrabTip::paintEvent(QWidget* w, QPaintEvent *)
{
    // QColor btnCl = KDrawHelper::getColorFromTheme("KScreenGrabTip", KDrawHelper::Prop_Background);
    QColor btnCl = QColor(Qt::black);
    QPainter painter(w);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath pathT, pathR;
    pathR.moveTo(MScreenTools::dpiScaled(16), MScreenTools::dpiScaled(TRI_Y));
    pathR.lineTo(MScreenTools::dpiScaled(16 + 6), 0);
    pathR.lineTo(MScreenTools::dpiScaled(16 + 6 + 6), MScreenTools::dpiScaled(TRI_Y));

    QSize sz = w->geometry().size();
    sz.rwidth() -= 0;
    sz.rheight() -= MScreenTools::dpiScaled(TRI_Y);
    QRect rrRc(QPoint(0, MScreenTools::dpiScaled(TRI_Y)), sz);
    pathT.addRoundedRect(rrRc, MScreenTools::dpiScaled(4), MScreenTools::dpiScaled(4));

    painter.fillPath(pathT, btnCl);
    painter.fillPath(pathR, btnCl);

    painter.setPen(QColor(54,66,90,50));
    painter.drawLine(rrRc.topLeft().x(), rrRc.topLeft().y() + 2, rrRc.bottomLeft().x(), rrRc.bottomLeft().y() - 2);
}

void KScreenGrabTip::setText(const QString& text)
{
    m_text = text;
}

// -----------------------------------------------------------------------
