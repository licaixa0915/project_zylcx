/**
 * \file screengrab_toolbar.cpp <Wed, 26 Feb 2014 10:20:55 +0000 (kic)>
 * \brief 
 */
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include "screengrab_toolbar.h"
#include "ksvgwidget.h"

inline qreal dpiScaled(qreal value)
{
#ifdef Q_OS_MACOS
    // On mac the DPI is always 72 so we should not scale it
    return value;
#else
    static const qreal scale = qreal(qt_defaultDpiX()) / 96.0;
    return value * scale;
#endif
}

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
	return QToolBar::sizeHint() + QSize(dpiScaled(8), 0);
}
