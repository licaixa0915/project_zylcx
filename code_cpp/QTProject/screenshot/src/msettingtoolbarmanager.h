#ifndef MSETTINGTOOLBARMANAGER_H
#define MSETTINGTOOLBARMANAGER_H

#include <QObject>

class KScreenGrabToolBar;

class MSettingToolBarManager : public QObject
{
public:
    static MSettingToolBarManager *instance();
    MSettingToolBarManager();
private:
    KScreenGrabToolBar*		m_toolbarSettings = nullptr;
};

#endif // MSETTINGTOOLBARMANAGER_H
