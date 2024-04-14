#include "screengrab_colorpicker.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
namespace KLiteStyle
{
	int dpiScaled(int value)
	{
		return value;
	}
};
namespace
{
	const QColor colorBlack = Qt::black;
	const QColor colorWhite = Qt::white;
	const QColor colorDarkGray = QColor(136, 136, 136);
	const QColor colorLightGray = QColor(204, 204, 204);
	const QColor colorDarkRed = QColor(128, 0, 0);
	const QColor colorRed = QColor(255, 0, 0);
	const QColor colorDarkYellow = Qt::darkYellow;
	const QColor colorYellow = Qt::yellow;
	const QColor colorDarkBlue = QColor(0, 36, 128);
	const QColor colorBlue = QColor(0, 72, 255);
	const QColor colorDarkGreen = QColor(0, 128, 0);
	const QColor colorGreen = QColor(0, 255, 0);
	const QColor colorDarkMagenta = QColor(117, 0, 128);
	const QColor colorMagenta = QColor(234, 0, 255);
	const QColor colorDarkCyan = Qt::darkCyan;
	const QColor colorCyan = Qt::cyan;
}

KScreenGrabColorPicker::KScreenGrabColorPicker(QWidget *parent)
	: QWidget(parent)
{
	// set background
	this->setAutoFillBackground(true);
	QPalette palette;
	palette.setBrush(backgroundRole(), QBrush(QColor(255, 255, 255)));
	setPalette(palette);

	setMouseTracking(true);

	setFixedWidth(KLiteStyle::dpiScaled(134));
	setFixedHeight(KLiteStyle::dpiScaled(24));

	m_colorSelect = colorRed;

	m_rcSelect = QRect(0, 0, KLiteStyle::dpiScaled(24), KLiteStyle::dpiScaled(24));
	m_rcBlack = m_rcSelect.adjusted(KLiteStyle::dpiScaled(30), 0, KLiteStyle::dpiScaled(30), 0);
	m_rcBlack.setBottomRight(m_rcBlack.topLeft() + QPoint(KLiteStyle::dpiScaled(10), KLiteStyle::dpiScaled(10)));
	
	int nGap = KLiteStyle::dpiScaled(13);
	m_rcWhite = m_rcBlack.adjusted(0, KLiteStyle::dpiScaled(12), 0, KLiteStyle::dpiScaled(12));

	m_rcDarkGray = m_rcBlack.adjusted(nGap, 0, nGap, 0);
	m_rcLightGray = m_rcWhite.adjusted(nGap, 0, nGap, 0);

	m_rcDarkRed = m_rcDarkGray.adjusted(nGap, 0, nGap, 0);
	m_rcRed = m_rcLightGray.adjusted(nGap, 0, nGap, 0);

	m_rcDarkYellow = m_rcDarkRed.adjusted(nGap, 0, nGap, 0);
	m_rcYellow = m_rcRed.adjusted(nGap, 0, nGap, 0);

	m_rcDarkGreen = m_rcDarkYellow.adjusted(nGap, 0, nGap, 0);
	m_rcGreen = m_rcYellow.adjusted(nGap, 0, nGap, 0);

	m_rcDarkBlue = m_rcDarkGreen.adjusted(nGap, 0, nGap, 0);
	m_rcBlue = m_rcGreen.adjusted(nGap, 0, nGap, 0);

	m_rcDarkMagenta = m_rcDarkBlue.adjusted(nGap, 0, nGap, 0);
	m_rcMagenta = m_rcBlue.adjusted(nGap, 0, nGap, 0);

	m_rcDarkCyan = m_rcDarkMagenta.adjusted(nGap, 0, nGap, 0);
	m_rcCyan = m_rcMagenta.adjusted(nGap, 0, nGap, 0);

	m_rcHover = QRect();

	m_rcEdge = geometry();
}

KScreenGrabColorPicker::~KScreenGrabColorPicker()
{

}

bool KScreenGrabColorPicker::event( QEvent* event )
{
	return QWidget::event(event);
}


bool KScreenGrabColorPicker::eventFilter( QObject* obj, QEvent* event )
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

	case QEvent::Leave:
		leaveEvent(event);

	default:
		break;
	}
	
	return QWidget::eventFilter(obj, event);
}

void KScreenGrabColorPicker::mousePressEvent( QMouseEvent* event )
{

}

void KScreenGrabColorPicker::mouseReleaseEvent( QMouseEvent* event )
{
	detectHoverRect(event->pos());

	if (!m_rcHover.isNull() && m_colorSelect != m_colorHover)
	{
		m_colorSelect = m_colorHover;
		update();
		emit(colorChanged(m_colorSelect));
	}
}

void KScreenGrabColorPicker::mouseMoveEvent( QMouseEvent* event )
{
	detectHoverRect(event->pos());
	update();
}

void KScreenGrabColorPicker::paintEvent( QPaintEvent* event )
{
	QPainter painter(this);
	// QColor btnCl = KDrawHelper::getColorFromTheme("KScreenGrabColorPicker", KDrawHelper::Prop_Background);
	QColor btnCl = QColor("#ebf1ff");

	painter.setPen(Qt::NoPen);
	painter.setBrush(btnCl);
	painter.drawRect(m_rcEdge);
	
	drawSelection(painter);

	drawColorRect(painter, m_rcBlack, colorBlack);
	drawColorRect(painter, m_rcWhite, colorWhite);
	painter.setPen(QColor(199, 209, 230));
	painter.drawRect(m_rcWhite);
	drawColorRect(painter, m_rcDarkGray, colorDarkGray);
	drawColorRect(painter, m_rcLightGray, colorLightGray);
	drawColorRect(painter, m_rcDarkRed, colorDarkRed);
	drawColorRect(painter, m_rcRed, colorRed);
	drawColorRect(painter, m_rcDarkYellow, colorDarkYellow);
	drawColorRect(painter, m_rcYellow, colorYellow);
	drawColorRect(painter, m_rcDarkGreen, colorDarkGreen);
	drawColorRect(painter, m_rcGreen, colorGreen);
	drawColorRect(painter, m_rcDarkBlue, colorDarkBlue);
	drawColorRect(painter, m_rcBlue, colorBlue);
	drawColorRect(painter, m_rcDarkMagenta, colorDarkMagenta);
	drawColorRect(painter, m_rcMagenta, colorMagenta);
	drawColorRect(painter, m_rcDarkCyan, colorDarkCyan);
	drawColorRect(painter, m_rcCyan, colorCyan);

	drawColorHover(painter, m_rcHover);
}

void KScreenGrabColorPicker::drawSelection( QPainter& painter )
{
	painter.setPen(Qt::NoPen);
	painter.setBrush(m_colorSelect);
	painter.drawRect(m_rcSelect.adjusted(0,0,-1,-1));

	painter.setPen(QColor(153, 153, 153));
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(m_rcSelect.adjusted(0,0,-1,-1));
}

void KScreenGrabColorPicker::drawColorRect( QPainter& painter, const QRect& rc, const QColor& color )
{
	painter.setPen(Qt::NoPen);
	painter.setBrush(color);
	painter.drawRect(rc.adjusted(1, 1, 0, 0));
}

void KScreenGrabColorPicker::drawColorHover( QPainter& painter, const QRect& rc )
{
	if (rc.isNull())
	{
		return;
	}
	painter.setPen(Qt::white);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(rc.adjusted(1, 1, -1, -1));

	painter.setPen(QColor(68, 72, 81));
	painter.drawRect(rc);
}

void KScreenGrabColorPicker::detectHoverRect( const QPoint& pt )
{
	if (m_rcBlack.contains(pt))
	{
		m_rcHover = m_rcBlack;
		m_colorHover = colorBlack;
	}
	else if (m_rcWhite.contains(pt))
	{
		m_rcHover = m_rcWhite;
		m_colorHover = colorWhite;
	}
	else if (m_rcDarkGray.contains(pt))
	{
		m_rcHover = m_rcDarkGray;
		m_colorHover = colorDarkGray;
	}
	else if (m_rcLightGray.contains(pt))
	{
		m_rcHover = m_rcLightGray;
		m_colorHover = colorLightGray;
	}
	else if (m_rcDarkRed.contains(pt))
	{
		m_rcHover = m_rcDarkRed;
		m_colorHover = colorDarkRed;
	}
	else if (m_rcRed.contains(pt))
	{
		m_rcHover = m_rcRed;
		m_colorHover = colorRed;
	}
	else if (m_rcDarkYellow.contains(pt))
	{
		m_rcHover = m_rcDarkYellow;
		m_colorHover = colorDarkYellow;
	}
	else if (m_rcYellow.contains(pt))
	{
		m_rcHover = m_rcYellow;
		m_colorHover = colorYellow;
	}
	else if (m_rcDarkGreen.contains(pt))
	{
		m_rcHover = m_rcDarkGreen;
		m_colorHover = colorDarkGreen;
	}
	else if (m_rcGreen.contains(pt))
	{
		m_rcHover = m_rcGreen;
		m_colorHover = colorGreen;
	}
	else if (m_rcDarkBlue.contains(pt))
	{
		m_rcHover = m_rcDarkBlue;
		m_colorHover = colorDarkBlue;
	}
	else if (m_rcBlue.contains(pt))
	{
		m_rcHover = m_rcBlue;
		m_colorHover = colorBlue;
	}
	else if (m_rcDarkMagenta.contains(pt))
	{
		m_rcHover = m_rcDarkMagenta;
		m_colorHover = colorDarkMagenta;
	}
	else if (m_rcMagenta.contains(pt))
	{
		m_rcHover = m_rcMagenta;
		m_colorHover = colorMagenta;
	}
	else if (m_rcDarkCyan.contains(pt))
	{
		m_rcHover = m_rcDarkCyan;
		m_colorHover = colorDarkCyan;
	}
	else if (m_rcCyan.contains(pt))
	{
		m_rcHover = m_rcCyan;
		m_colorHover = colorCyan;
	}
	else
	{
		m_rcHover = QRect();
		m_colorHover = QColor();
	}
}

void KScreenGrabColorPicker::setColor( const QColor& color )
{
	m_colorSelect = color;
	update();
}

void KScreenGrabColorPicker::leaveEvent( QEvent* event )
{
	m_rcHover = QRect();
	m_colorHover = QColor();
	update();
}
