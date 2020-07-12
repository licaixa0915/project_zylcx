#ifndef FLOATTOOLWIDGET_H
#define FLOATTOOLWIDGET_H

#include <QWidget>
#include "editgrabpicturewidget.h"
#include "capturebase.h"
class FloatToolWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FloatToolWidget(QWidget *parent = nullptr);
    void setCaptureInfo(QPixmap &catureImage, QRect captureRect);
protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
private:
    void InitWidget();
signals:
    void savePicClicked();
public slots:
    void onToolBtnClicked();
private:
    QPoint m_lastMousePosition;
    bool m_moving;
private:
    EditGrabPictureWidget *m_pEditGrabPictureWidget;
    QPixmap m_catureImage;
    QRect m_captureRect;
    QMap<QString, EditType> m_editMap;
};

#endif // FLOATTOOLWIDGET_H
