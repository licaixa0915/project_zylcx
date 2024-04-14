#ifndef __SCREENGRAB_PEN_H__
#define __SCREENGRAB_PEN_H__
#include <QWidget>
class KScreenGrabPen : public QWidget
{
	Q_OBJECT

public:
	KScreenGrabPen(int size, QWidget *parent);
	~KScreenGrabPen();
	void selected(bool select);

protected:
	bool event(QEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void enterEvent(QEvent* event);
	void leaveEvent(QEvent* event);

signals:
	void penSizeChanged(int size);

private:
	int		m_nSize;
	QRect	m_rcEdge;
	QRect	m_rcCenter;
	bool	m_fSelect;
	bool	m_fHover;
	bool	m_fMouseDown;
};

#endif // __SCREENGRAB_PEN_H__
