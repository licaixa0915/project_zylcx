#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QStack>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    int calculate();
    ~Widget();
private slots:
    void onClickedButton();

private:
    Ui::Widget *ui;
    static int equFlag;
};

#endif // WIDGET_H
