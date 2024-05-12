/**
 * \file screengrab_toolbar.cpp <Wed, 26 Feb 2014 10:20:55 +0000 (kic)>
 * \brief 
 */
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QDialog>
#include "screengrab_toolbar.h"
#include "mscreentools.h"
#include "ksvgwidget.h"
#include "mscreengrabtip.h"
#include "kscreengrabdialog.h"

//////////////////////////////////////////////////////////////////////////
KScrnGrabToolButton::KScrnGrabToolButton(QWidget* parent, const QString& text, bool bAction)
	: QWidget(parent)
	, m_text(text)
	, m_bHover(false)
	, m_bInited(false)
	, m_bPress(false)
	, m_label(nullptr)
	, m_textColor("")
	, m_bCheck(false)
	, m_bAction(bAction)
	, m_bLimitFree(false)
	, m_lbFree(nullptr)
{
	setCursor(Qt::ArrowCursor);
	setToolTip(text);
}

KScrnGrabToolButton::~KScrnGrabToolButton()
{

}

void KScrnGrabToolButton::actionClick()
{
	m_bPress = false;
	emit clicked();
	if (m_bAction)
		m_bCheck = true;

	update();
}
void KScrnGrabToolButton::setLimitFree(bool bFree)
{
	m_bLimitFree = bFree;
}

void KScrnGrabToolButton::setChecked(bool bCheck)
{
	m_bCheck = bCheck;
	update();
}

bool KScrnGrabToolButton::isChecked()
{
	return m_bCheck;
}

void KScrnGrabToolButton::setShowText(bool bShow)
{
	if (m_label)
	{
		if (bShow)
			m_label->show();
		else
			m_label->hide();
	}
}

void KScrnGrabToolButton::setTextColor(const QString& strColor)
{
	m_textColor = strColor;
}

void KScrnGrabToolButton::setIcon(const QString& iconName)
{
	m_iconName = iconName;
}

void KScrnGrabToolButton::init()
{
	if (!m_bInited)
	{
		initUi();
		m_bInited = true;
	}
}

void KScrnGrabToolButton::initUi()
{
	QHBoxLayout *pLayout = new QHBoxLayout(this);
	pLayout->setContentsMargins(5, 5, 5, 5);
	pLayout->setSpacing(4);

	KSvgWidget *lbImg = new KSvgWidget(this);
	m_lbFree = new KSvgWidget(this);
	m_lbFree->setFixedSize(21, 10);
	m_lbFree->hide();
	lbImg->setFixedSize(16, 16);
	if (m_bLimitFree)
	{
		QString strFile = "://icons_scg/16x16/small.svg";
		m_lbFree->load(strFile);
		m_lbFree->show();
		QRect rc = this->geometry();
		m_lbFree->move(13, 13);
	}
	QString iconName = m_iconName;
	lbImg->load(iconName);
	pLayout->addWidget(lbImg);

	m_label = new QLabel(m_text, this);

	pLayout->addWidget(m_label);
}

QString KScrnGrabToolButton::resPath(const QString &name)
{
	QString resPathStr = "://icons_png/";
	return resPathStr + name;
}

QString KScrnGrabToolButton::resSvgPath(const QString& name)
{
	QString resPathStr = "://icons_svg/16x16/";

	return resPathStr + name;
}

void KScrnGrabToolButton::paintEvent(QPaintEvent *e)
{
	// QString bgHoverName = QString(KDrawHelper::Prop_Background) + "-hover";
	// QString bgDownName = QString(KDrawHelper::Prop_Background) + "-down";
	// QColor cl = KDrawHelper::getColorFromTheme("KScrnGrabToolButton", KDrawHelper::Prop_Background);
	// QColor clHover = KDrawHelper::getColorFromTheme("KScrnGrabToolButton", bgHoverName);
	// QColor clDown = KDrawHelper::getColorFromTheme("KScrnGrabToolButton", bgDownName);

	QColor cl = QColor("#FFFFFF");
	QColor clHover = QColor("#F5F5F5");
	QColor clDown = QColor("#EEEEEE");

	if (m_bHover)
		cl = clHover;
	if (m_bPress)
		cl = clDown;
	if (m_bCheck)
		cl = clDown;

	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(cl);
	painter.drawRoundedRect(rect(), 2, 2);
}

void KScrnGrabToolButton::showEvent(QShowEvent *e)
{
	init();
}

void KScrnGrabToolButton::enterEvent(QEnterEvent *e)
{
	m_bHover = true;
	update();
}

void KScrnGrabToolButton::leaveEvent(QEvent* e)
{
	m_bHover = false;
	update();
}

void KScrnGrabToolButton::mousePressEvent(QMouseEvent *e)
{
	m_bPress = true;
	if (m_bAction)
		m_bCheck = true;

	update();
}

void KScrnGrabToolButton::mouseReleaseEvent(QMouseEvent *e)
{
	m_bPress = false;
	emit clicked();
	if (m_bAction)
		m_bCheck = true;

	update();
}

//////////////////////////////////////////////////////////////////////////
KScreenGrabToolBar::KScreenGrabToolBar(QWidget* parent)
	: QToolBar(parent)
{
	// set background
	this->setAutoFillBackground(true);
	QPalette palette;
	palette.setBrush(backgroundRole(), QBrush(QColor(235, 241, 255)));
	setPalette(palette);

	setWindowFlags(Qt::FramelessWindowHint);
}

KScreenGrabToolBar::~KScreenGrabToolBar()
{

}

QSize KScreenGrabToolBar::sizeHint() const
{
    return QToolBar::sizeHint() + QSize(MScreenTools::dpiScaled(8), 0);
}



KScreenGrabToolBarHeader::KScreenGrabToolBarHeader(KScreenGrabDialog* parent)
    : KScreenGrabToolBar(parent)
    , m_moveWidget(nullptr)
    , m_moving(false)
    , m_dialog(parent)
    , m_tooltip(nullptr)
{
    QRect rc(0, 0, MScreenTools::dpiScaled(450), MScreenTools::dpiScaled(88));
    QRect screenGeo = QApplication::primaryScreen()->geometry();
    rc.translate(screenGeo.x() + (screenGeo.width() - rc.width()) / 2, screenGeo.y() + 20);
    setGeometry(rc);

    m_moveWidget = new QWidget(this);
    QSize fixedSize = QSize(MScreenTools::dpiScaled(22), MScreenTools::dpiScaled(88));
    rc = QRect(size().width() - fixedSize.width(), 0, fixedSize.width(), fixedSize.height());
    rc.adjust(MScreenTools::dpiScaled(1), MScreenTools::dpiScaled(1), MScreenTools::dpiScaled(-1), MScreenTools::dpiScaled(-1));
    m_moveWidget->setGeometry(rc);
    m_moveWidget->installEventFilter(this);
    m_moveWidget->setCursor(Qt::SizeAllCursor);
}
KScreenGrabToolBarHeader::~KScreenGrabToolBarHeader()
{

}
bool KScreenGrabToolBarHeader::eventFilter(QObject * obj, QEvent * e)
{
    if (obj == m_moveWidget)
    {
        if (e->type() == QEvent::MouseMove)
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(e);
            QPoint pt = geometry().topLeft() + me->pos() - m_down;
            move(pt);
            m_dialog->update();
            return true;
        }
        else if (e->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* me = static_cast<QMouseEvent*>(e);
            m_moving = true;
            m_down = me->pos();
            return true;
        }
        else if (e->type() == QEvent::MouseButtonRelease)
        {
            m_down = QPoint();
            m_moving = false;
            return true;
        }
        else if (e->type() == QEvent::Paint)
        {
            QPaintEvent* pe = static_cast<QPaintEvent*>(e);
            QPainter painter(m_moveWidget);
            painter.drawPixmap(QRect(QPoint(0, 0), m_moveWidget->size()), QPixmap(":icons_svg/other/move.svg"));
        }
    }
    else
    {
        QWidget* control = qobject_cast<QWidget*>(obj);
        if (control)
        {
            if (e->type() == QEvent::Enter)
            {
                if (!m_tooltip)
                {
                    m_tooltip = new KScreenGrabTip(m_dialog);
                }
                show();
                QRect rc = control->geometry();
                QPoint bl = mapTo(m_dialog, rc.bottomLeft());
                bl.rx() += rc.width() / 2.f - MScreenTools::dpiScaled(16);
                bl.ry() += MScreenTools::dpiScaled(8);
                m_tooltip->setGeometry(QRect(bl, QSize(MScreenTools::dpiScaled(226), MScreenTools::dpiScaled(116))));
                m_tooltip->setText(control->toolTip());
                m_tooltip->reset(control);
                m_tooltip->show();
                m_dialog->update();
                return true;
            }
            else if (e->type() == QEvent::Leave)
            {
                if (m_tooltip)
                    m_tooltip->hide();
                m_dialog->update();
                return true;
            }
            else if (e->type() == QEvent::ToolTip)
            {
                return true;
            }
        }
    }
    return KScreenGrabToolBar::eventFilter(obj, e);
}

void KScreenGrabToolBarHeader::hideTooltip()
{
    if (m_tooltip)
        m_tooltip->hide();
}

