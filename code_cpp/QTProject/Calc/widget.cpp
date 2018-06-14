#include "widget.h"
#include "ui_widget.h"
#include <QAction>
#include <QDebug>

enum OP_PRIORITY
{
    E_OP_0,
    E_OP_1,
    E_OP_2,

    E_OP_BUTT
};

int Widget::equFlag = 0;//保存当前是不是等号的特殊处理
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->pushButton_0, SIGNAL(clicked(bool)), this, SLOT(onClickedButton()));
    connect(ui->pushButton_1, SIGNAL(clicked(bool)), this, SLOT(onClickedButton()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(onClickedButton()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(onClickedButton()));
    connect(ui->pushButton_4, SIGNAL(clicked(bool)), this, SLOT(onClickedButton()));
    connect(ui->pushButton_5, SIGNAL(clicked(bool)), this, SLOT(onClickedButton()));
    connect(ui->pushButton_6, SIGNAL(clicked(bool)), this, SLOT(onClickedButton()));
    connect(ui->pushButton_7, SIGNAL(clicked(bool)), this, SLOT(onClickedButton()));
    connect(ui->pushButton_8, SIGNAL(clicked(bool)), this, SLOT(onClickedButton()));
    connect(ui->pushButton_9, SIGNAL(clicked(bool)), this, SLOT(onClickedButton()));

    connect(ui->pushButton_add, SIGNAL(clicked(bool)),this, SLOT(onClickedButton()));
    connect(ui->pushButton_del, SIGNAL(clicked(bool)),this, SLOT(onClickedButton()));
    connect(ui->pushButton_mul, SIGNAL(clicked(bool)),this, SLOT(onClickedButton()));
    connect(ui->pushButton_div, SIGNAL(clicked(bool)),this, SLOT(onClickedButton()));
    connect(ui->pushButton_back, SIGNAL(clicked(bool)),this, SLOT(onClickedButton()));
    connect(ui->pushButton_clear, SIGNAL(clicked(bool)),this, SLOT(onClickedButton()));
    connect(ui->pushButton_equ, SIGNAL(clicked(bool)),this, SLOT(onClickedButton()));
    connect(ui->pushButton_left, SIGNAL(clicked(bool)),this, SLOT(onClickedButton()));
    connect(ui->pushButton_right, SIGNAL(clicked(bool)),this, SLOT(onClickedButton()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onClickedButton()
{
    QPushButton* btn = dynamic_cast<QPushButton*>(sender());
    if (btn == ui->pushButton_equ)
    {
        this->equFlag = 1;
        int iRest = calculate();
        char stRes[20] = {0};
        ui->lineEdit->setText(ui->lineEdit->text()+btn->text());
        ui->lineEdit->setText(ui->lineEdit->text()+ itoa(iRest, stRes, 10));
    }
    else if(btn == ui->pushButton_back)
    {
        ui->lineEdit->backspace();
    }
    else if(btn == ui->pushButton_clear)
    {
        ui->lineEdit->clear();
    }
    else
    {
        if (1 == this->equFlag)
        {
            this->equFlag = 0;
            ui->lineEdit->setText(btn->text());
        }
        else
            ui->lineEdit->setText(ui->lineEdit->text()+btn->text());
    }
}
int getOperatorPriority(char op)
{
    switch (op)
    {
        case '+':
        case '-':
            return E_OP_1;
        case '/':
        case '*':
            return E_OP_2;
        case '(':
        case ')':
            /*为了代码的通用行，这里括号也给排了优先级*/
            return E_OP_0;
        default:
            printf("operator is error!\n");
            return E_OP_BUTT;
    }
}

template<class T, class T1>
void calc(QStack<T>&pstOperand, QStack<T1>&pstOperator)
{
    int ret = 0;
    int optd1 = 0;
    int optd2 = 0;
    char op = 0;

    op = pstOperator.top();
    if (op != '(')
    {
        optd1 = pstOperand.pop();
        optd2 = pstOperand.pop();
        switch (op)
        {
            /*注意操作数的顺序*/
            case '+':
                ret = optd2 + optd1;
                break;
            case '-':
                ret = optd2 - optd1;
                break;
            case '*':
                ret = optd2 * optd1;
                break;
            case '/':
                ret = optd2 / optd1;
                break;
            default:
                printf("op error!\n");
                return;
        }
        pstOperand.push(ret);
    }
    /*如果是(则直接出栈即可*/
    pstOperator.pop();

    return;
}
template<class T, class T1>
void dealOperator(QStack<T>&pstOperand, QStack<T1>&pstOperator, char op)
{
    int priority = 0;

    priority = getOperatorPriority(op);

    while ((!pstOperator.empty())
        && (priority <= getOperatorPriority(pstOperator.top())))
    {
        /*遇到右括号的话，一直运算到(，记得最后一个左括号要出栈*/
        if ((')' == op) && (pstOperator.top() == '('))
        {
            calc<int, char>(pstOperand, pstOperator);
            break;
        }
        calc<int, char>(pstOperand, pstOperator);
    }

    /*运算解释之后，除了)其他的都要入栈*/
    if (')' != op)
    {
        pstOperator.push(op);
    }
    return;
}
int Widget::calculate()
{
    QString exp = ui->lineEdit->text();
    QStack<int> pstOperand;
    QStack<char>pstOperator;
    int data = 0;
    qDebug()<<exp;

    for (int i = 0; i < exp.size(); )
    {
        qDebug()<<exp[i].toLatin1();
        if ((exp[i].toLatin1() >= '0') && (exp[i].toLatin1() <= '9'))
        {
            data = 0;
            while(exp[i].toLatin1() >= '0' && exp[i].toLatin1() <= '9' && i < exp.size())
            {
                data = data * 10 + exp[i].toLatin1() - '0';
                i++;
            }
            pstOperand.push(data);
            continue;
        }
        else
        {
            switch (exp[i].toLatin1())
            {
                case '+':
                case '-':
                case '*':
                case '/':
                case ')':
                    dealOperator<int, char>(pstOperand, pstOperator, exp[i].toLatin1());
                    break;
                case '(':
                    /*左括号直接入栈*/
                    pstOperator.push(exp[i].toLatin1());
                    break;
                case ' ':
                    i++;
                    continue;
                default:
                    qDebug("expression is error!");
                    return 0;

            }
        }
        i++;
    }
    /* 将栈中的数据全部运算 */
    while (!pstOperator.empty())
    {
        calc<int, char>(pstOperand, pstOperator);
    }
    /*最终操作数栈中的值就是最终的结果值*/
    return pstOperand.pop();
}
