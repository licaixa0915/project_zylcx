/**
 * \file screengrab_toolbar.h <Wed, 26 Feb 2014 10:19:50 +0000 (kic)>
 * \brief 
 */


#ifndef __SCREENGRAB_TOOLBAR_H__
#define __SCREENGRAB_TOOLBAR_H__
#include <QToolBar>
QT_BEGIN_NAMESPACE
class QWidget;
class QLabel;
QT_END_NAMESPACE
class KSvgWidget;
class KScreenGrabTip;
class KScrnGrabToolButton : public QWidget
{
	Q_OBJECT
public:
	KScrnGrabToolButton(QWidget* parent,const QString& text, bool bAction = false);
	~KScrnGrabToolButton();

	void setTextColor(const QString& strColor);
	void setIcon(const QString& iconName);

	void setChecked(bool bCheck);
	bool isChecked();
	void setShowText(bool bShow);
	void setLimitFree(bool bFree);
	void actionClick();
private:
	void init();
	void initUi();
	QString resPath(const QString& name);
	QString resSvgPath(const QString& name);

protected:
	virtual void paintEvent(QPaintEvent *e) override;
	virtual void showEvent(QShowEvent *e) override;
    virtual void enterEvent(QEnterEvent *e) override;
	virtual void leaveEvent(QEvent* e) override;
	virtual void mousePressEvent(QMouseEvent *e) override;
	virtual void mouseReleaseEvent(QMouseEvent *e) override;

signals:
	void clicked();

private:
	QString m_iconName;
	QString m_textColor;
	QString m_text;
	bool m_bInited;
	bool m_bHover;
	bool m_bPress;
	bool m_bCheck;
	bool m_bAction;
	QLabel* m_label;
	bool m_bLimitFree;
	KSvgWidget *m_lbFree;
};

class KScreenGrabToolBar : public QToolBar
{
	Q_OBJECT

public:
	KScreenGrabToolBar(QWidget* parent);
	~KScreenGrabToolBar();

protected:
	virtual QSize sizeHint() const override;
};

class KScreenGrabDialog;
class KScreenGrabToolBarHeader : public KScreenGrabToolBar
{
    Q_OBJECT

public:
    KScreenGrabToolBarHeader(KScreenGrabDialog* parent);
    ~KScreenGrabToolBarHeader();

public:
    virtual bool eventFilter(QObject *, QEvent *);

public:
    void hideTooltip();

private:
    QWidget* m_moveWidget;
    QPoint m_down;
    bool m_moving;
    KScreenGrabDialog* m_dialog;
    KScreenGrabTip* m_tooltip;
};

#endif // __SCREENGRAB_TOOLBAR_H__
