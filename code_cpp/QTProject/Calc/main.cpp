#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("Calc");
    w.resize(500,600);
    w.show();

    return a.exec();
}
