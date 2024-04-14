#ifndef MSCREENSHOTTRAYMAINWINDOW_H
#define MSCREENSHOTTRAYMAINWINDOW_H

#include <QWidget>
#include <QSystemTrayIcon>
class KScreenGrabTrayMainWindow : public QWidget
{
    Q_OBJECT
public:
    KScreenGrabTrayMainWindow();
    ~KScreenGrabTrayMainWindow();
public slots:
    void onScreenShotClick();
protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    void initUi();
};
class KScreenGrabTrayMainWindow;
class KScreenGrabTrayManager : public QObject
{
    Q_OBJECT

public:
    KScreenGrabTrayManager();
    ~KScreenGrabTrayManager();
public slots:
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
private:
    void initTray();
    void showTrayMainWindow();
private:
    QSystemTrayIcon *m_pSysTray = nullptr;
    KScreenGrabTrayMainWindow *m_pMainWidget = nullptr;
};

#endif // MSCREENSHOTTRAYMAINWINDOW_H
