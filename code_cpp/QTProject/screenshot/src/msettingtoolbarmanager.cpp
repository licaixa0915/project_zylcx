#include "msettingtoolbarmanager.h"

static MSettingToolBarManager *instance()
{
    static MSettingToolBarManager s_settingToolBarManager;
    return &s_settingToolBarManager;
}

MSettingToolBarManager::MSettingToolBarManager()
    : QObject()
{

}
