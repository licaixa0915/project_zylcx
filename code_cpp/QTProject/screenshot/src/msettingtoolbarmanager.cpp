#include "msettingtoolbarmanager.h"
#include "screengrab_toolbar.h"
#include "mscreentools.h"
#include "screengrab_pen.h"
#include "screengrab_colorpicker.h"
#include "screengrab_fontsetting.h"

MSettingToolBarManager* MSettingToolBarManager::instance()
{
    static MSettingToolBarManager s_settingToolBarManager;
    return &s_settingToolBarManager;
}

MSettingToolBarManager::MSettingToolBarManager()
    : QObject()
{

}
void MSettingToolBarManager::initStyleSheet()
{
    QColor bgCl = QColor("#FFFFFF");
    QColor bgBorder = QColor("#EEEEEE");
    QString strBg = QString("rgba(%1, %2, %3, %4)").arg(bgCl.red()).arg(bgCl.green()).arg(bgCl.blue()).arg(bgCl.alpha());
    QString strBorder = QString("rgba(%1, %2, %3, %4)").arg(bgBorder.red()).arg(bgBorder.green()).arg(bgBorder.blue()).arg(bgBorder.alpha());
    m_toolbarSettings->setStyleSheet(
        QString("QToolBar{background-color:%4;border:1px solid %5;border-radius:%1px;padding:%2px;}"
            "QToolButton{border-style:flat;margin-top:%3px;margin-bottom:%3px;")
        .arg(MScreenTools::dpiScaled(4))
        .arg(MScreenTools::dpiScaled(0))
        .arg(MScreenTools::dpiScaled(0))
        .arg(strBg)
        .arg(strBorder));
}

void MSettingToolBarManager::init(QWidget *parent)
{
    if (m_toolbarSettings)
        return;

    m_toolbarSettings = new KScreenGrabToolBar(parent);
    m_toolbarSettings->setFixedHeight(MScreenTools::dpiScaled(30));
    m_toolbarSettings->setFixedWidth(MScreenTools::dpiScaled(245));

    // pen width [small, middle, large]
    m_penSmall = new KScreenGrabPen(KScreenGrabPen::FixSizeRound, parent, MScreenTools::dpiScaled(penWidthSmall));
    m_penMiddle = new KScreenGrabPen(KScreenGrabPen::FixSizeRound, parent, MScreenTools::dpiScaled(penWidthMiddle));
    m_penLarge = new KScreenGrabPen(KScreenGrabPen::FixSizeRound, parent, MScreenTools::dpiScaled(penWidthLarge));
    m_penSmall->setCursor(Qt::ArrowCursor);
    m_penMiddle->setCursor(Qt::ArrowCursor);
    m_penLarge->setCursor(Qt::ArrowCursor);
    connect(m_penSmall, &KScreenGrabPen::penSizeChanged, this, &MSettingToolBarManager::penSizeChanged);
    connect(m_penMiddle, &KScreenGrabPen::penSizeChanged, this, &MSettingToolBarManager::penSizeChanged);
    connect(m_penLarge, &KScreenGrabPen::penSizeChanged, this, &MSettingToolBarManager::penSizeChanged);

    QWidget* paddingWidget = new QWidget(parent);
    paddingWidget->setFixedWidth(1);
    m_toolbarSettings->addWidget(paddingWidget);

    m_actionPenSmall = m_toolbarSettings->addWidget(m_penSmall);
    m_actionPenSmall->setVisible(true);
    m_actionPenSmall->setCheckable(true);
    m_actionPenSmall->setObjectName("penSmall");

    m_actionPenMiddle = m_toolbarSettings->addWidget(m_penMiddle);
    m_actionPenMiddle->setVisible(true);
    m_actionPenMiddle->setCheckable(true);
    m_actionPenMiddle->setObjectName("penMiddle");

    m_actionPenLarge = m_toolbarSettings->addWidget(m_penLarge);
    m_actionPenLarge->setVisible(true);
    m_actionPenLarge->setCheckable(true);
    m_actionPenLarge->setObjectName("penLarge");

    //round value button
    m_roundValueBtn = new KScreenGrabPen(KScreenGrabPen::VariableRound, parent);
    m_roundValueBtn->setCursor(Qt::ArrowCursor);
    m_actionRoundValue = m_toolbarSettings->addWidget(m_roundValueBtn);
    m_actionRoundValue->setObjectName("roundValue");
    m_actionRoundValue->setVisible(false);
    connect(m_roundValueBtn, &KScreenGrabPen::penSizeChanged, this, &MSettingToolBarManager::penSizeChanged);

    //rect button
    m_fixedRectBtn = new KScreenGrabPen(KScreenGrabPen::FixSizeRect, parent);
    m_fixedRectBtn->setCursor(Qt::ArrowCursor);
    m_actionFixedRect = m_toolbarSettings->addWidget(m_fixedRectBtn);
    m_actionFixedRect->setObjectName("fixedRect");
    m_actionFixedRect->setVisible(false);

    // font setting
    m_fontSetting = new KScreenGrabFontSetting(parent);
    m_fontSetting->setProperty("qtspyName", "GrabFontSetting");
    m_fontSetting->setCursor(Qt::ArrowCursor);
    connect(m_fontSetting, &KScreenGrabFontSetting::fontSizeChanged,
            this, &MSettingToolBarManager::fontSizeChanged);


    // separator
    m_actionSeparator = m_toolbarSettings->addSeparator();
    m_toolbarSettings->widgetForAction(m_actionSeparator)->setCursor(Qt::ArrowCursor);
    m_toolbarSettings->widgetForAction(m_actionSeparator)->setFixedSize(QSize(10, 24));

    // color picker
    m_colorPicker = new KScreenGrabColorPicker(parent);
    m_colorPicker->setCursor(Qt::ArrowCursor);
    m_colorPicker->setProperty("qtspyName", QString("GrabColorPicker"));
    connect(m_colorPicker, &KScreenGrabColorPicker::colorChanged,
            this, &MSettingToolBarManager::colorChanged);

    m_actionColor = m_toolbarSettings->addWidget(m_colorPicker);
    m_actionColor->setObjectName("color");
    m_actionColor->setVisible(true);

    initStyleSheet();
}

int MSettingToolBarManager::getPenSize()
{

}

void MSettingToolBarManager::onEditModeChange(EditType editMode)
{
    static QMap<EditType, QStringList> s_editModeToolbar = 
    {
        { editNone, QStringList()},
        { editFilter, QStringList()},
        { editRect, QStringList() << "penSmall" << "penMiddle"<<"penLarge"<<"separator"<<"color"},
        { editEllipse, QStringList() << "penSmall" << "penMiddle"<<"penLarge"<<"separator"<<"color"},
        { editArrow, QStringList() << "penSmall" << "penMiddle"<<"penLarge"<<"separator"<<"color"},
        { editBrush, QStringList() << "penSmall" << "penMiddle"<<"penLarge"<<"separator"<<"color"},
        { editMosaic, QStringList() << "penSmall" << "roundValue"<<"fixedRect"},
        { editText, QStringList() << "font"<<"color"},
    };
    
    QStringList result = s_editModeToolbar.value(editMode);
    for (QAction *action : m_toolbarSettings->actions())
    {
        bool needVisible = result.contains(action->objectName());
        action->setVisible(needVisible);
    }
}
int MSettingToolBarManager::settingToolBarHeight()
{
    if (m_toolbarSettings)
        return m_toolbarSettings->height();
    return 0;
}
void MSettingToolBarManager::hideSettingToolBar()
{
    if (m_toolbarSettings && m_toolbarSettings->isVisible())
        m_toolbarSettings->hide();
}

void MSettingToolBarManager::showSettingToolBar()
{
    if (m_toolbarSettings)
        m_toolbarSettings->show();
}

void MSettingToolBarManager::relocateToolBar(QPoint ptTarget, int toolbarHeight, bool bShowSetting, QRect selectionRect, bool bSelectionBottom)
{
    if (bShowSetting && m_toolbarSettings)
	{
		QPoint ptSetting = ptTarget;
        int nGap = MScreenTools::dpiScaled(4);
        int nOffset = MScreenTools::dpiScaled(45);
		if (bSelectionBottom)
		{
			ptSetting.setY(ptTarget.y() + nOffset);
		}
        else if (ptTarget.y() < toolbarHeight + nGap / 2)
		{
			ptSetting.setY(ptTarget.y() + nOffset); // do nothing
		}
        else if (ptTarget.y() + toolbarHeight + m_toolbarSettings->height() > selectionRect.top() + nGap)
		{
            ptSetting.setY(ptTarget.y() - toolbarHeight - m_toolbarSettings->height() -  nGap + nOffset);
		}
		m_toolbarSettings->show();
		m_toolbarSettings->move(ptSetting);
	}
	else
	{
		m_toolbarSettings->hide();
	}
}

void MSettingToolBarManager::selectPenWidth(int width)
{
    switch (width)
	{
	case penWidthSmall:
		m_penSmall->selected(true);
		m_penMiddle->selected(false);
		m_penLarge->selected(false);
		break;
	case penWidthMiddle:
		m_penSmall->selected(false);
		m_penMiddle->selected(true);
		m_penLarge->selected(false);
		break;
	case penWidthLarge:
		m_penSmall->selected(false);
		m_penMiddle->selected(false);
		m_penLarge->selected(true);
		break;
	default:
		break;
	}
}

void MSettingToolBarManager::setColorPickerColor(QColor color)
{
    m_colorPicker->setColor(color);
}
