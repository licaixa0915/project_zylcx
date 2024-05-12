#ifndef MTOOLBARHEADER_H
#define MTOOLBARHEADER_H

#include <QObject>
#include <QAction>
#include "mcommon.h"

class KScreenGrabToolBarHeader;
class KScreenGrabDialog;

class MToolBarHeaderManager : public QObject
{
    Q_OBJECT
public:
    explicit MToolBarHeaderManager(QObject *parent = nullptr);
    void init(KScreenGrabDialog* parent, DrawShape drawShape);
    void showToolBarHeader();
    void hideToolBarHeader();
    void toggleDrawAction();
    const QSize* getTooltipSizeHint(QWidget*);
    KScreenGrabToolBarHeader * toolBarHeader();
    void showToolTip();
signals:
    void drawShapeChanged(DrawShape);
    void sigToggleDrawAction();
private slots:
    void onActionDrawRect();
    void onActionDrawRoundedRect();
    void onActionDrawEllipse();
    void onActionDrawPolygon();
private:
    KScreenGrabToolBarHeader*	m_toolbarHeader = nullptr;
    QAction*		m_actionDrawRect = nullptr;
    QAction*		m_actionDrawRoundedRect = nullptr;
    QAction*		m_actionDrawEllipse = nullptr;
    QAction*		m_actionDrawPolygon = nullptr;
    QMap<QWidget*, QSize> m_tooltipSizeHints;
    DrawShape m_drawShape;
};

#endif // MTOOLBARHEADER_H
