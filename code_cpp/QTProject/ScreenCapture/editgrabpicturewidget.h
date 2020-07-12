#ifndef EDITGRABPICTUREWIDGET_H
#define EDITGRABPICTUREWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "capturebase.h"
typedef struct ShapesInfo
{
    EditType editType;
    QPoint beginPos;
    QPoint endPos;
    QColor penColor;
    int penWidth;
}S_ShapesInfo;

class EditGrabPictureWidget : public QWidget
{
    Q_OBJECT
public:

    explicit EditGrabPictureWidget(QWidget *parent = nullptr);
    ~EditGrabPictureWidget();
    void setPicture(QPixmap catureImage);
    void setEditType(EditType editType);
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *);//重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *);//重写鼠标释放事件
    void mouseMoveEvent(QMouseEvent *);//重写鼠标移动事件
private:
    QPixmap m_catureImage;
    EditType m_editType;
    bool m_leftPress;
    QPoint m_beginPos;
    QPoint m_endPos;
    QVector<QVector<QPoint> > m_lines;//线条集合(一条线条可包含多个线段)
    QVector<QRect> m_rects;//矩形集合
    QVector<QRect> m_ellipses;//椭圆集合
    QVector<QRect>  m_line;//直线集合
    QVector<QString>  m_texts;//文字集合
    QVector<QPoint>  m_tpoints;//文字位置集合
    QVector<S_ShapesInfo>  m_shapes;//图形类型集合，用于撤回功能
};

#endif // EDITGRABPICTUREWIDGET_H
