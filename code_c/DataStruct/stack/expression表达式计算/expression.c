#include <stdio.h>
#include "seqstack.h"

enum OP_PRIORITY
{
    E_OP_0,
    E_OP_1,
    E_OP_2,

    E_OP_BUTT
};

int farsight_getOperatorPriority(char op)
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

void farsight_calc(S_SEQSTACK *pstOperand, S_SEQSTACK *pstOperator)
{
    int ret = 0;
    int optd1 = 0;
    int optd2 = 0;
    char op = 0;

    op = farsight_getTop(pstOperator);
    if (op != '(')
    {
        optd1 = farsight_popSeqStack(pstOperand);
        optd2 = farsight_popSeqStack(pstOperand);
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
        farsight_pushSeqStack(pstOperand, ret);
    }
    /*如果是(则直接出栈即可*/
    farsight_popSeqStack(pstOperator);
    
    return;
}

void farsight_dealOperator(S_SEQSTACK *pstOperand, S_SEQSTACK *pstOperator, char op)
{
    int priority = 0;

    priority = farsight_getOperatorPriority(op);
    
    while ((!farsight_isEmptyStack(pstOperator)) 
        && (priority <= farsight_getOperatorPriority(farsight_getTop(pstOperator))))
    {
        /*遇到右括号的话，一直运算到(，记得最后一个左括号要出栈*/
        if ((')' == op) && (farsight_getTop(pstOperator) == '('))
        {
            farsight_calc(pstOperand, pstOperator);
            break;
        }
        farsight_calc(pstOperand, pstOperator);
    }

    /*运算解释之后，除了)其他的都要入栈*/
    if (')' != op)
    {
        farsight_pushSeqStack(pstOperator, op);
    }
    return;
}

int farsight_calcExpreesion(char * pExpression)
{
    int ret = 0;
    char *p = pExpression;
    int data = 0;
    S_SEQSTACK *pstOperand = NULL;//操作数栈
    S_SEQSTACK *pstOperator = NULL;//运算符栈
    int i;
    printf("%s\n", pExpression);//为了后边调试方便,将表达式的内容打印出来
    
    pstOperand = farsight_createEmptyStack();
    pstOperator = farsight_createEmptyStack();
    while (*p)
    {
        if ((*p >= '0') && (*p <= '9'))
        {
            data = 0;
            
            while(*p >= '0' && *p <= '9')
            {
                data = data * 10 + *p - '0';
                p++;
            }
            /* 得到操作数则入操作数栈 */
            farsight_pushSeqStack(pstOperand, data);
            continue;
        }
        else
        {
            switch (*p)
            {
                case '+':
                case '-':
                case '*':
                case '/':
                case ')':
                    farsight_dealOperator(pstOperand, pstOperator, *p);
                    break;
                case '(':
                    /*左括号直接入栈*/
                    farsight_pushSeqStack(pstOperator, *p);
                    break;
                case ' ':
                    p++;
                    continue;
                default:
                    printf("expression is error!\n");
                    return;
                    
            }
        }


        p++;
    }
    
    /* 将栈中的数据全部运算 */
    while (!farsight_isEmptyStack(pstOperator))
    {
        farsight_calc(pstOperand, pstOperator);
    }
    /*最终操作数栈中的值就是最终的结果值*/
    ret = farsight_popSeqStack(pstOperand);
    return ret;
}

