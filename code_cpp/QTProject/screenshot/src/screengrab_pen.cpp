#include "screengrab_pen.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#define DEFAULT_VALUE 10
KScreenGrabPen::KScreenGrabPen(Type type, QWidget *parent, int size)
    : QPushButton(parent)
    , m_type(type)
{
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(Qt::FramelessWindowHint);

    if ((type == VariableRound || type == VariableRect || type == FixSizeRect) && (size == 0))
    {
        m_nSize = DEFAULT_VALUE;
    }
    else
        m_nSize = size;
	m_fSelect = false;
	m_fHover = false;
	m_fMouseDown = false;

	setMouseTracking(true);

	setFixedWidth(24);
	setFixedHeight(24);

	m_rcEdge = geometry();
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
    int x = (m_rcEdge.width() - m_nSize + 1) / 2 + m_rcEdge.x();
    int y = (m_rcEdge.height() - m_nSize + 1) / 2 + m_rcEdge.y();

    QRect center = QRect(x - 1, y - 1, m_nSize + 1, m_nSize + 1);

    if (m_type == FixSizeRound || m_type == VariableRound)
        painter.drawEllipse(center);
    else
        painter.drawRect(center);
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

void KScreenGrabPen::wheelEvent(QWheelEvent *event)
{
    if (m_fSelect && m_type == VariableRound)
    {
        int before = m_nSize;
        if (event->angleDelta().y() > 0) {
            // 鼠标向上滚动
            if (m_nSize < 50)
                ++m_nSize;
        } else {
            // 鼠标向下滚动
            if (m_nSize > 8)
                --m_nSize;
        }
        if (before != m_nSize)
        {
            update();
            emit(penSizeChanged(m_nSize));
        }
    }

    return QWidget::wheelEvent(event);
}
