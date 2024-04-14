#include "src/mscreenshottraymainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false); //禁止最后一个窗口关闭之后退出应用程序
    a.setOrganizationName("licaixia");
    //设置应用图标
    // a.setWindowIcon(QIcon(":/moneybox.ico"));

    QTranslator eptoolsTrans, qtTrans;
    QString strEptoolsQmPath = QCoreApplication::applicationDirPath() + "/../Resources/office6/translations/screengrab.qm";
    eptoolsTrans.load(strEptoolsQmPath);

    qtTrans.load(":/qt_zh_CN.qm");

    a.installTranslator(&eptoolsTrans);
    a.installTranslator(&qtTrans);
    KScreenGrabTrayManager *pTrayManager = new KScreenGrabTrayManager();

    int exitcode = 0;
    exitcode = a.exec();

    delete pTrayManager;
    return exitcode;
}
