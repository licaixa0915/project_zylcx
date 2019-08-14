#ifndef CLOCK_H
#define CLOCK_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QDebug>
namespace Ui {
class Clock;
}

class Clock : public QWidget
{
    Q_OBJECT

public:
    explicit Clock(QWidget *parent = 0);
    ~Clock();
    void paintEvent(QPaintEvent *event);

private:
    Ui::Clock *ui;
};

#endif // CLOCK_H
