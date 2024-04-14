#ifndef SCREENGRAB_PIN_H
#define SCREENGRAB_PIN_H
#include <QWidget>
#include "screengrab_menu.h"
#include "kscreengrabdialog.h"
#include <QElapsedTimer>
QT_BEGIN_NAMESPACE
class QHBoxLayout;
QT_END_NAMESPACE
class KScreenGrabPinMenu : public KScreenGrabMenu
{
	Q_OBJECT

public:
	KScreenGrabPinMenu(QWidget *parent);

	virtual void hideEvent(QHideEvent *) override;
	virtual void focusOutEvent(QFocusEvent *) override;

signals:
	void hiding();
	void focusOut();
};

class KScreenGrabPin : public QWidget
{
	Q_OBJECT

public:
	KScreenGrabPin(KScreenGrabDialog::DrawShape ds);
	~KScreenGrabPin();

public:
	void setImage(const QImage& image);
	void show(const QRect& rc);

protected:
	virtual bool eventFilter(QObject *, QEvent *) override;
	virtual void contextMenuEvent(QContextMenuEvent*) override;
	virtual void showEvent(QShowEvent *) override;

private:
	virtual void leaveEvent(QEvent *) override;
    virtual void enterEvent(QEnterEvent *) override;
	virtual void mousePressEvent(QMouseEvent *) override;
	virtual void mouseMoveEvent(QMouseEvent *) override;
	virtual void mouseReleaseEvent(QMouseEvent *) override;
	virtual void keyPressEvent(QKeyEvent*) override;

private slots:
	void onHiding();
	void onMenuFocusOut();
	void onActionCopy();
	void onActionShadow();
	void onActionSave();
	void onActionClose();

private:
	void hideMenu();
	void switchShadow();

private:
	QWidget* m_mainWidget;
	QWidget* m_imageWidget;
	QImage m_image;
	KScreenGrabMenu* m_menu;
	QAction* m_actionShadow;
	QAction* m_actionCopy;
	QAction* m_actionSave;
	QAction* m_actionClose;
	QPoint m_down;
	bool m_moving;
	QElapsedTimer m_countdown;
	bool m_shadowOn;
	QGraphicsDropShadowEffect* m_eff;
	QPushButton* m_pbShadow;
	QPushButton* m_pbCopy;
	QPushButton* m_pbSave;
	QPushButton* m_pbClose;
	KScreenGrabDialog::DrawShape m_drawShape;
	bool m_invisible;
	QHBoxLayout* m_menuLayout;
	QRect m_showRect;
};

#endif // SCREENGRAB_PIN_H
