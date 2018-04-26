#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    execl("./calc", "calc", argv[1], argv[2], argv[3], NULL);
    return 0;
}
