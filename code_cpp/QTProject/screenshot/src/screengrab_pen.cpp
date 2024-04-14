#include "screengrab_pen.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>

KScreenGrabPen::KScreenGrabPen(int size, QWidget *parent)
	: QWidget(parent)
{
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(Qt::FramelessWindowHint);
	m_nSize = size;
	m_fSelect = false;
	m_fHover = false;
	m_fMouseDown = false;

	setMouseTracking(true);

	setFixedWidth(24);
	setFixedHeight(24);

	m_rcEdge = geometry();


	int x = (m_rcEdge.width() - m_nSize + 1) / 2 + m_rcEdge.x();
	int y = (m_rcEdge.height() - m_nSize + 1) / 2 + m_rcEdge.y();

	m_rcCenter = QRect(x - 1, y - 1, m_nSize + 1, m_nSize + 1);
}

KScreenGrabPen::~KScreenGrabPen()
{

}

bool KScreenGrabPen::event( QEvent* event )
{
	return QWidget::event(event);
}

bool KScreenGrabPen::eventFilter( QObject* obj, QEvent* event )
{
	switch (event->type())
	{
	case QEvent::MouseMove:
		mouseMoveEvent(static_cast<QMouseEvent*>(event));
		break;

	case QEvent::MouseButtonPress:
		mousePressEvent(static_cast<QMouseEvent*>(event));
		break;

	case QEvent::MouseButtonRelease:
		mouseReleaseEvent(static_cast<QMouseEvent*>(event));
		break;

	case QEvent::Enter:
		enterEvent(event);
		break;

	case QEvent::Leave:
		leaveEvent(event);

	default:
		break;
	}

	return QWidget::event(event);
}

void KScreenGrabPen::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	// QColor Cl = KDrawHelper::getColorFromTheme("KScreenGrabPen", KDrawHelper::Prop_Background);
	// QColor text = KDrawHelper::getColorFromTheme("KScreenGrabPen",  KDrawHelper::Prop_Text);
	// QColor hover = KDrawHelper::getColorFromTheme("KScreenGrabPen", "background-hover");
	QColor Cl = QColor("#FFFFFF");
	QColor text = QColor("#0D0D0D");
	QColor hover = QColor("#E6E6E6");

	painter.setPen(Qt::NoPen);
	painter.setBrush(Cl);
	painter.drawRoundedRect(m_rcEdge, 4, 4);

	if (m_fMouseDown)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(hover);
		painter.drawRoundedRect(m_rcEdge, 4, 4);
	}
	else if (m_fSelect || m_fHover)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(hover);
		painter.drawRoundedRect(m_rcEdge, 4, 4);
	}

	painter.setRenderHint(QPainter::Antialiasing, true);
	
	painter.setPen(Qt::NoPen);
	painter.setBrush(text);
	painter.drawEllipse(m_rcCenter);
}

void KScreenGrabPen::mousePressEvent(QMouseEvent* event)
{
	m_fMouseDown = true;
	update();
}

void KScreenGrabPen::mouseReleaseEvent(QMouseEvent* event)
{
	m_fMouseDown = false;
	update();
	emit(penSizeChanged(m_nSize));
}

void KScreenGrabPen::mouseMoveEvent(QMouseEvent* event)
{
	//m_fHover = true;
	//update();
}

void KScreenGrabPen::selected( bool select )
{
	m_fSelect = select;
	update();
}

void KScreenGrabPen::enterEvent( QEvent* event )
{
	m_fHover = true;
	update();
}

void KScreenGrabPen::leaveEvent( QEvent* event )
{
	m_fHover = false;
	update();
}
