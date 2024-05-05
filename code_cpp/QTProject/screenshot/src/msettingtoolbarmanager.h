#ifndef MSETTINGTOOLBARMANAGER_H
#define MSETTINGTOOLBARMANAGER_H

#include <QObject>
#include "mcommon.h"
class KScreenGrabToolBar;
class KScreenGrabColorPicker;
class KScreenGrabPen;
class QAction;
class KScreenGrabFontSetting;

class MSettingToolBarManager : public QObject
{
    Q_OBJECT
public:
    enum PenWidth
    {
        penWidthSmall = 1,
        penWidthMiddle = 3,
        penWidthLarge = 7,
    };
    enum ControlType
    {
        controlTypeRoundPen,
        controlTypeRect
    };
    static MSettingToolBarManager *instance();
    MSettingToolBarManager();    
    void init(QWidget * parent);
    int getPenSize();
    void hideSettingToolBar();
    void showSettingToolBar();
    int settingToolBarHeight();
    void relocateToolBar(QPoint ptTarget, int toolbarHeight, bool bShowSetting, QRect selectionRect, bool bSelectionBottom);
    void selectPenWidth(int width);
    void setColorPickerColor(QColor color);
private:
    void initStyleSheet();
signals:
    void penSizeChanged(int size);
    void colorChanged(const QColor& color);
	void fontSizeChanged(int size);
public slots:
    void onEditModeChange(EditType editMode);

private:
    KScreenGrabToolBar*		m_toolbarSettings = nullptr;
    KScreenGrabColorPicker* m_colorPicker = nullptr;
    KScreenGrabPen*			m_penSmall = nullptr;
    KScreenGrabPen*			m_penMiddle = nullptr;
    KScreenGrabPen*			m_penLarge = nullptr;
    KScreenGrabPen*         m_roundValueBtn = nullptr;
    KScreenGrabPen*         m_fixedRectBtn = nullptr;
    KScreenGrabFontSetting* m_fontSetting = nullptr;

    QAction*		m_actionPenSmall = nullptr;
    QAction*		m_actionPenMiddle = nullptr;
    QAction*		m_actionPenLarge = nullptr;
    QAction*		m_actionFont = nullptr;
    QAction*		m_actionSeparator = nullptr;
    QAction*		m_actionColor = nullptr;
    QAction*		m_actionRoundValue = nullptr;
    QAction*		m_actionFixedRect = nullptr;
};

#endif // MSETTINGTOOLBARMANAGER_H
