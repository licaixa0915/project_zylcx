#include "floatbutton.h"
#include "floattoolwidget.h"
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QDebug>

FloatToolWidget::FloatToolWidget(QWidget *parent) : QWidget(parent)
{
    m_pEditGrabPictureWidget = nullptr;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(300, 35);
    InitWidget();
}

void FloatToolWidget::InitWidget()
{
    m_editMap.insert("矩形", EditType_DrawRect);
    m_editMap.insert("椭圆", EditType_DrawEclipse);
    m_editMap.insert("保存", EditType_SavePic);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QMap<QString, EditType>::Iterator it = m_editMap.begin();
    while (it != m_editMap.end())
    {
        FloatButton *ptestBtn = new FloatButton(this);
        ptestBtn->setText(it.key());
        ptestBtn->setObjectName(it.key());
        connect(ptestBtn, SIGNAL(clicked()), this, SLOT(onToolBtnClicked()));
        mainLayout->addWidget(ptestBtn);
        it++;
    }

    this->setLayout(mainLayout);
}

void FloatToolWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_moving = true;
        m_lastMousePosition = event->globalPos();
    }
}

void FloatToolWidget::mouseMoveEvent(QMouseEvent* event)
{
    if( event->buttons().testFlag(Qt::LeftButton) && m_moving)
    {
        this->move(this->pos() + (event->globalPos() - m_lastMousePosition));
        m_lastMousePosition = event->globalPos();
    }
}

void FloatToolWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_moving = false;
    }
}

void FloatToolWidget::onToolBtnClicked()
{
    if (!m_pEditGrabPictureWidget)
    {
        m_pEditGrabPictureWidget = new EditGrabPictureWidget();
        m_pEditGrabPictureWidget->move(m_captureRect.topLeft());
        m_pEditGrabPictureWidget->setFixedSize(m_captureRect.width(), m_captureRect.height());
        m_pEditGrabPictureWidget->show();
        m_pEditGrabPictureWidget->activateWindow();
        m_pEditGrabPictureWidget->setPicture(m_catureImage);
    }
    QObject*object = sender();
    QString strObjectName = object->objectName();
    if (strObjectName =="保存")
        this->hide();

    QMap<QString, EditType>::Iterator it;
    it = m_editMap.find(strObjectName);
    if (it != m_editMap.end())
    {
        qDebug()<<"it.value():"<<it.value();
        m_pEditGrabPictureWidget->setEditType(it.value());
    }
}

void FloatToolWidget::setCaptureInfo(QPixmap &catureImage, QRect captureRect)
{
    m_catureImage = catureImage;
    m_captureRect = captureRect;
}


