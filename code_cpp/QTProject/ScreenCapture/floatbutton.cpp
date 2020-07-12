#include "floatbutton.h"

FloatButton::FloatButton(QWidget *parent)
    :QPushButton(parent)
{
    this->setStyleSheet("QPushButton {background-color: transparent;border:none; }");
    this->setFixedSize(30, 30);
}

FloatButton::~FloatButton()
{

}
