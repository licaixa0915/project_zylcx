#include "mscreentools.h"

namespace MScreenTools
{
    qreal dpiScaled(qreal value)
    {
    #ifdef Q_OS_MACOS
        // On mac the DPI is always 72 so we should not scale it
        return value;
    #else
        static const qreal scale = qreal(qt_defaultDpiX()) / 96.0;
        return value * scale;
    #endif
    }
};
