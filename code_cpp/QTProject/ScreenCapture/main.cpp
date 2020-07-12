#include "screencapturemainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScreenCaptureMainWindow w;
    w.show();

    return a.exec();
}
