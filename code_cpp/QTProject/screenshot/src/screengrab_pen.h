#ifndef __SCREENGRAB_PEN_H__
#define __SCREENGRAB_PEN_H__
#include <QPushButton>
class KScreenGrabPen : public QPushButton
{
	Q_OBJECT

public:
    enum Type
    {
        FixSizeRound,
        VariableRound,
        FixSizeRect,
        VariableRect,

        Invalid
    };
    KScreenGrabPen(Type type, QWidget *parent, int size = 0);
	~KScreenGrabPen();
	void selected(bool select);

protected:
	bool event(QEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent *event) override;
	void enterEvent(QEvent* event);
	void leaveEvent(QEvent* event);

signals:
	void penSizeChanged(int size);

private:
	int		m_nSize;
	QRect	m_rcEdge;
	bool	m_fSelect;
	bool	m_fHover;
	bool	m_fMouseDown;
    Type    m_type;
};

#endif // __SCREENGRAB_PEN_H__
