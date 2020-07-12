#include "editgrabpicturewidget.h"
#include <QPainter>
#include <QDebug>
#include <QFileDialog>
#include <QApplication>
#include <QScreen>

EditGrabPictureWidget::EditGrabPictureWidget(QWidget *parent) : QWidget(parent)
{
    m_leftPress = false;
    m_editType = EditType_Init;
    this->setWindowFlags(Qt::FramelessWindowHint);
    qDebug()<<"EditGrabPictureWidget()"<<this;
}


void EditGrabPictureWidget::setPicture(QPixmap catureImage)
{
    m_catureImage = catureImage;
    update();
}

EditGrabPictureWidget::~EditGrabPictureWidget()
{
}

void EditGrabPictureWidget::setEditType(EditType editType)
{
    m_editType = editType;
    if (m_editType == EditType_SavePic)
    {
        this->hide();
        //弹出文件保存对话框
        QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), "new.jpg", "Image (*.jpg *.png *.bmp)");
        if (fileName.length() > 0)
        {
            QPixmap pixmap(size());//新建窗体大小的pixmap
            QScreen *screen = QGuiApplication::primaryScreen();
            if (!screen)
                return;
            pixmap.setDevicePixelRatio(screen->devicePixelRatio());
            QPainter painter(&pixmap);//将pixmap作为画布
            painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);//设置绘画区域、画布颜色
            this->render(&painter);//将窗体渲染到painter，再由painter画到画布
            pixmap.copy(QRect(0, 0, size().width(), size().height())).save(fileName);
        }
    }
}

void EditGrabPictureWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)//当鼠标左键按下
    {
        qDebug()<<"mousePressEvent Qt::LeftButton m_editType = "<<m_editType;
        if (m_editType == EditType_DrawRect || m_editType == EditType_DrawEclipse)//矩形
        {
            m_leftPress = true;//左键按下标志
            m_beginPos = e->pos();//记录拖拽开始的坐标位置,方便计算位移
            QRect lastRect;
            lastRect.setTopLeft(m_beginPos);
            m_rects.append(lastRect);
            S_ShapesInfo shapesInfo;
            shapesInfo.editType = m_editType;
            shapesInfo.beginPos = e->pos();//记录拖拽开始的坐标位置,方便计算位移
            shapesInfo.endPos = e->pos();
            m_shapes.append(shapesInfo);
        }
    }
}


void EditGrabPictureWidget::mouseMoveEvent(QMouseEvent *e)
{
    setCursor(Qt::ArrowCursor);//恢复原始光标形状
    if (m_leftPress)
    {
        if (m_editType == EditType_DrawRect || m_editType == EditType_DrawEclipse)//矩形
        {
            m_endPos = e->pos();//记录拖拽结束的坐标位置,方便计算位移
            m_shapes.last().endPos = e->pos();//记录拖拽结束的坐标位置,方便计算位移
            update();//触发窗体重绘
        }
    }
}

void EditGrabPictureWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_leftPress)
    {
        if (m_editType == EditType_DrawRect)//矩形
        {
            m_shapes.last().endPos = e->pos();//记录拖拽结束的坐标位置,方便计算位移
            m_leftPress = false;//标志左键释放
        }
    }
}

void EditGrabPictureWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(this->rect(), m_catureImage);
    for (int index = 0; index < m_shapes.size(); ++index)//控制用户当前所绘图形总数
    {
        if (m_shapes.at(index).editType == EditType_DrawRect || m_shapes.at(index).editType == EditType_DrawEclipse)
        {
            int x = m_shapes.at(index).beginPos.x();
            int y = m_shapes.at(index).beginPos.y();
            int width = m_shapes.at(index).endPos.x() - m_shapes.at(index).beginPos.x();
            int height = m_shapes.at(index).endPos.y() - m_shapes.at(index).beginPos.y();
            p.save();
            p.setPen(Qt::red);
            if (m_shapes.at(index).editType == EditType_DrawRect)
                p.drawRect(x, y, width, height);

            if (m_shapes.at(index).editType == EditType_DrawEclipse)
                p.drawEllipse(x, y, width, height);
            p.restore();
        }
    }
}
