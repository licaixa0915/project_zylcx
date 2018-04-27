#include <stdio.h>
#include "seqstack.h"
#include "expression.h"

int main(int argc, const char *argv[])
{
    char szExpression[120] = {0};
    int ret = 0;

    //scanf("%s", szExpression);

    gets(szExpression);
    ret = farsight_calcExpreesion(szExpression);
    printf("%d\n", ret);

    return 0;
}
