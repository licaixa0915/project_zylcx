#ifndef __SCREENGRAB_FONTSETTING_H__
#define __SCREENGRAB_FONTSETTING_H__
#include <QWidget>
#include <QListView>
class KLiteComboBox;
class KScreenGrabFontSetting : public QWidget
{
	Q_OBJECT

public:
	KScreenGrabFontSetting(QWidget *parent);
	~KScreenGrabFontSetting();

protected:
	bool event(QEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);

private slots:
	void currentIndexChanged(const QString& text);

	void onSkinChanged();
signals:
	void fontSizeChanged(int size);

private:
	// KLiteComboBox*	m_comboBoxSize;
	QListView*  m_listViewSize;
	int			m_nSize;
	QIcon		m_icon;
};

#endif // __SCREENGRAB_FONTSETTING_H__
