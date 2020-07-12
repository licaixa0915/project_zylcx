#ifndef CAPTURESCREEN_H
#define CAPTURESCREEN_H

#include <QWidget>
#include <QPainter>

enum CaptureState{
	InitCapture = 0,
	BeginCaptureImage,
	FinishCaptureImage,
	BeginMoveCaptureArea,
	FinishMoveCaptureArea,
	BeginMoveStretchRect,
	FinishMoveStretchRect
}; //进行截屏的状态;

enum StretchRectState{
	NotSelect = 0,
	TopLeftRect,
	TopRightRect,
	BottomLeftRect,
	BottomRightRect,
	LeftCenterRect,
	TopCenterRect,
	RightCenterRect,
	BottomCenterRect
};// 当前鼠标所在顶点状态;

class CaptureScreen : public QWidget
{
    Q_OBJECT
public:
    explicit CaptureScreen(QWidget *parent = nullptr);
    ~CaptureScreen();

signals:
    void signalCompleteCature(QPixmap catureImage);
    void showGrabToolWidget(QPixmap catureImage, QRect captureRect);
    void hideGrabToolWidget();

private:
    void initWindow();
	void initStretchRect();
    void loadBackgroundPixmap();
    QRect getRect(const QPoint &beginPoint, const QPoint &endPoint);
    QRect getMoveRect();
	QRect getStretchRect();
    bool isPressPointInSelectRect(QPoint mousePressPoint);
    QRect getSelectRect();
    
    QPoint getMovePoint();
	StretchRectState getStrethRectState(QPoint point);
	void setStretchCursorStyle(StretchRectState stretchRectState);
	void drawCaptureImage();
    void drawEndPointImage();
    void drawSelectRectInfo();
	void drawStretchRect();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_loadPixmap, m_capturePixmap;
    int m_screenwidth;
    int m_screenheight;
    QPoint m_beginPoint, m_endPoint , m_beginMovePoint , m_endMovePoint;
    QPainter m_painter;
    CaptureState m_currentCaptureState;
    QRect m_currentSelectRect;
    qreal m_devicePixelRatio;
	// 选中矩形8个顶点小矩形;
	QRect m_topLeftRect, m_topRightRect, m_bottomLeftRect, m_bottomRightRect;
	QRect m_leftCenterRect, m_topCenterRect, m_rightCenterRect, m_bottomCenterRect;
	// 当前鼠标所在顶点状态;
	StretchRectState m_stretchRectState;
};

#endif // CAPTURESCREEN_H
