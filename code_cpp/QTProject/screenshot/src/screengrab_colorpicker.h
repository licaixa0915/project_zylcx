#ifndef __SCREENGRAB_COLORPICKER_H__
#define __SCREENGRAB_COLORPICKER_H__
#include <QWidget>
class KScreenGrabColorPicker : public QWidget
{
	Q_OBJECT

public:
	explicit KScreenGrabColorPicker(QWidget *parent);
	~KScreenGrabColorPicker();

	void setColor(const QColor& color);

protected:
	bool event(QEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void leaveEvent(QEvent* event);

private:
	void drawSelection(QPainter& painter);
	void drawColorRect(QPainter& painter, const QRect& rc, const QColor& color);
	void drawColorHover(QPainter& painter, const QRect& rc);
	void detectHoverRect(const QPoint& pt);

signals:
	void colorChanged(const QColor& newColor);

private:
	QColor m_colorSelect;
	QRect  m_rcHover;
	QColor m_colorHover;
	
	QRect m_rcSelect;
	QRect m_rcBlack;
	QRect m_rcWhite;
	QRect m_rcDarkGray;
	QRect m_rcLightGray;
	QRect m_rcDarkRed;
	QRect m_rcRed;
	QRect m_rcDarkYellow;
	QRect m_rcYellow;
	QRect m_rcDarkGreen;
	QRect m_rcGreen;
	QRect m_rcDarkBlue;
	QRect m_rcBlue;
	QRect m_rcDarkMagenta;
	QRect m_rcMagenta;
	QRect m_rcDarkCyan;
	QRect m_rcCyan;

	QCursor m_cursor;

	QRect m_rcEdge;
};

#endif // __SCREENGRAB_COLORPICKER_H__h
