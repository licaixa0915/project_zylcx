#ifndef __SCREENGRAB_MENU_H__
#define __SCREENGRAB_MENU_H__
#include <QPointer>
#include <QWidget>
class KScreenGrabMenu : public QWidget
{
	Q_OBJECT

	enum MenuItemType
	{
		ItemAction,
		ItemSeparator,
	};

	struct MenuItem
	{
		MenuItemType type;
		QPointer<QAction> action;
		QRect rect;
		bool bLimitFree;
		QString iconName;
	};

public:
	explicit KScreenGrabMenu(QWidget *parent);
	~KScreenGrabMenu();

	void addAction(QAction* action, bool bLimitFree = false);
	void addSeparator(void);
	void resetHoveredAction(void);
	int actionCount();

	void setItemCheckable(bool bCheckable);
protected:
	bool event(QEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void keyPressEvent(QKeyEvent *event);

private:
	void drawMenu(QPainter& painter);
	void drawSelection(QPainter& painter);
	void detectHoveredItem(const QPoint& pt);

private:
	QList<MenuItem> m_items;
	QRect m_hoverRect;
	QPointer<QAction> m_hoverAction;
	int m_actionCount;
	bool m_bMenuItemCheckable;
};

#endif // __SCREENGRAB_MENU_H__
