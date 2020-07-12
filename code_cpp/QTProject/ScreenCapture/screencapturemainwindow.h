#ifndef SCREENCAPTUREMAINWINDOW_H
#define SCREENCAPTUREMAINWINDOW_H
#include "capturescreen.h"
#include "floattoolwidget.h"
#include <QMainWindow>

namespace Ui {
class ScreenCaptureMainWindow;
}

class ScreenCaptureMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScreenCaptureMainWindow(QWidget *parent = nullptr);
    ~ScreenCaptureMainWindow();
public slots:
    void onCaptureClicked();
    void onFloatToolClicked();
    void onShowGrabToolWidget(QPixmap catureImage, QRect captureRect);
    void onHideGrabToolWidget();
private:
    Ui::ScreenCaptureMainWindow *ui;
    CaptureScreen *m_pCaptureWidget;
    FloatToolWidget *m_pFloatToolWidget;
};

#endif // SCREENCAPTUREMAINWINDOW_H
