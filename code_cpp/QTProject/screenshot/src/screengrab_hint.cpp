#include "screengrab_hint.h"
#include <QPainter>
#include <QApplication>
namespace 
{
	int dpiScaled(int value)
	{
		return value;
	}
};

KScreenGrabHint::KScreenGrabHint(QWidget *parent, int timeout /*= -1*/)
	: QWidget(parent)
	, m_timeout(timeout)
{

	this->setFixedWidth(dpiScaled(140));
	this->setFixedHeight(dpiScaled(32));

	m_effect = new QGraphicsOpacityEffect(this);
	m_effect->setOpacity(1.0);
	this->setGraphicsEffect(m_effect);

	m_timer = new QTimer(this);
}

KScreenGrabHint::~KScreenGrabHint()
{
	if (NULL != m_effect)
	{
		delete m_effect;
		m_effect = NULL;
	}

	if (NULL != m_timer)
	{
		delete m_timer;
		m_timer = NULL;
	}
}

void KScreenGrabHint::display()
{
	m_effect->setOpacity(1.0);
	m_startTime = QDateTime::currentDateTime();
	if (NULL != m_timer)
	{
		connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
		m_timer->start(50);
	}
	else
	{
		m_effect->setOpacity(0.0);
	}
}

void KScreenGrabHint::setText(const QString& text)
{
	m_strHintText = text;
}

void KScreenGrabHint::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	// QColor bgCl = KDrawHelper::getColorFromTheme("KScreenGrabHint", KDrawHelper::Prop_Background);
	// QColor borderCl = KDrawHelper::getColorFromTheme("KScreenGrabHint", KDrawHelper::Prop_Border);
	// QColor textCl = KDrawHelper::getColorFromTheme("KScreenGrabHint", KDrawHelper::Prop_Text);
	QColor bgCl = QColor("#FFFFFF");
	QColor borderCl = QColor(Qt::blue);
	QColor textCl = QColor("#E6E6E6");
	QSize szFont = QSize(painter.device()->logicalDpiX() * 8 / 72 + 1,
		painter.device()->logicalDpiY() * 8 / 72 + 1);

	QFont font = qApp->font("QToolTip");
	font.setPointSize(8);
	QSize szText = QSize(dpiScaled(szFont.width() * 1.1 * m_strHintText.length() / 1.8), dpiScaled(szFont.height() * 2));

	int polygonX = width() / 2;
	painter.setPen(Qt::NoPen);
	painter.setBrush(bgCl);
	painter.drawRect(QRect(0, dpiScaled(3), width(), height() - dpiScaled(2)));
	QPoint pts[3] = {QPoint(polygonX, 0), QPoint(polygonX - dpiScaled(3), dpiScaled(3)), QPoint(polygonX + dpiScaled(3), dpiScaled(3))};
	painter.drawPolygon(pts, 3);

	painter.setPen(borderCl);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(QRect(0, dpiScaled(3), width(), dpiScaled(30)));
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.drawLine(QPoint(polygonX, 0), QPoint(polygonX - dpiScaled(3), dpiScaled(3)));
	painter.drawLine(QPoint(polygonX, 0), QPoint(polygonX + dpiScaled(3), dpiScaled(3)));
	painter.setPen(bgCl);
	painter.setRenderHint(QPainter::Antialiasing, false);
	painter.drawLine(QPoint(polygonX - dpiScaled(3), dpiScaled(3)), QPoint(polygonX + dpiScaled(3), dpiScaled(3)));

	QTextOption option;
	option.setAlignment(Qt::AlignCenter);
	painter.setPen(textCl);
	painter.setBrush(Qt::NoBrush);
	painter.drawText(QRect(0, dpiScaled(4), width(), dpiScaled(30)), m_strHintText, option);
}

void KScreenGrabHint::onTimer()
{
	int ms = m_startTime.msecsTo(QDateTime::currentDateTime());
	int time = m_timeout > 0 ? m_timeout : 1500;

	if (ms <= 0)
	{
		; // do nothing
	}
	else if (ms <= time)
	{
		m_effect->setOpacity(1.0);
	}
	else if (ms <= (time + 500) )
	{
		m_effect->setOpacity(1.0 - (qreal)(ms - time) / 500);
	}
	else
	{
		m_effect->setOpacity(0.0);
		this->hide();
		this->move(-1000, -1000);
		if (NULL != m_timer)
		{
			m_timer->stop();
		}
	}
	update();
}


//------------------------------------------------------------------------
KScreenGrabDoneHint::KScreenGrabDoneHint(QWidget *parent, int timeout /*= -1*/)
    : QWidget(parent)
    , m_timeout(timeout)
{

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setFixedWidth(dpiScaled(102));
    this->setFixedHeight(dpiScaled(90));
    m_timer = new QTimer(this);
    m_icon = QIcon("://res/16x16/success.svg");
}

KScreenGrabDoneHint::~KScreenGrabDoneHint()
{
    if (NULL != m_timer)
    {
        delete m_timer;
        m_timer = NULL;
    }
}

void KScreenGrabDoneHint::display()
{
    m_startTime = QDateTime::currentDateTime();
    if (NULL != m_timer)
    {
        connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
        m_timer->start(50);
    }
    else
    {
    }
}

void KScreenGrabDoneHint::setText(const QString& text)
{
    m_strHintText = text;
}

void KScreenGrabDoneHint::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    // QColor bgCl = KDrawHelper::getColorFromTheme("KScreenGrabDoneHint", KDrawHelper::Prop_Background);
    // QColor textCl = KDrawHelper::getColorFromTheme("KScreenGrabDoneHint", KDrawHelper::Prop_Text, Qt::white);
    QColor bgCl = QColor("#0D0D0D");
    QColor textCl = QColor("#FFFFFF");

    painter.setPen(Qt::NoPen);
    painter.setBrush(bgCl);
    painter.drawRoundedRect(rect(), 2, 2);

    QRect rcIcon = QRect(rect().left() + 35, rect().top() + 14, 32, 32);
    QRect rcText = QRect(rect().left() + 9, rect().top() + 54, 84, 22);
    if (!m_icon.isNull())
    {
        m_icon.paint(&painter, rcIcon);
    }

    QFont ft("PingFang SC");
    ft.setPixelSize(12);
    QTextOption option;
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setPen(textCl);
    painter.setFont(ft);
    painter.setBrush(Qt::NoBrush);
    painter.drawText(rcText, m_strHintText);
}

void KScreenGrabDoneHint::onTimer()
{
    int ms = m_startTime.msecsTo(QDateTime::currentDateTime());
    int time = m_timeout > 0 ? m_timeout : 1500;

    if (ms <= 0)
    {
        ; // do nothing
    }
    else if (ms <= time)
    {

    }
    else if (ms <= (time + 500) )
    {

    }
    else
    {
        this->hide();
        this->move(-1000, -1000);
        if (NULL != m_timer)
        {
            m_timer->stop();
        }
    }
    update();
}
