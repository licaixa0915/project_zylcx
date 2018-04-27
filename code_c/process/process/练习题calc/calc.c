#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}

int del(int a, int b)
{
    return a - b;
}

int mul(int a, int b)
{
    return a*b;
}

int div(int a, int b)
{
    return a/b;
}

int main(int argc, const char *argv[])
{
    if (argc < 4)
    {
        printf("para error\n");
        return 0;
    }

    if (0 == strcmp(argv[1], "add"))
    {
        printf("%d", add(atoi(argv[2]), atoi(argv[3])));
    }
    if (0 == strcmp(argv[1], "del"))
    {
        printf("%d", del(atoi(argv[2]), atoi(argv[3])));
    }
    if (0 == strcmp(argv[1], "mul"))
    {
        printf("%d", mul(atoi(argv[2]), atoi(argv[3])));
    }
    if (0 == strcmp(argv[1], "div"))
    {
        printf("%d", div(atoi(argv[2]), atoi(argv[3])));
    }
    printf("\n");
    return 0;
}
