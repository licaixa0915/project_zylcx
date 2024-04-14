#ifndef KWINDOWINFO_H
#define KWINDOWINFO_H

#include <QtCore>
#include <QtWidgets>

namespace KWindowUtil {
void setupDialogTitleBar(QWidget* window);
void setWindowTopLevel(QWidget* w, bool bTop = true);
}

class KWindowInfo : public QObject
{
public:
	KWindowInfo(QObject* parent, QWidget* topWidget);
	~KWindowInfo();
	void getWindowList(QWidget* widget);
	QRect getWindowRect(const QPoint& pt);
	QPixmap getWindowPixmap(const QPoint& pt, const QRect& sniffRect, const QRect& sgRect,
							int screenOffset, bool& bFullScreen);
private:
	CFArrayRef m_windowList;
	long m_windowId;
	long m_sniffId;
	QRect m_rectSniff;
};

#endif // KWINDOWINFO_H
