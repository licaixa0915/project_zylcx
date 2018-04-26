#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "para error: %s secfile desfile\n", argv[0]);
        return 0;
    }

    FILE *fpS = fopen(argv[1], "r"); 
    FILE *fpD = fopen(argv[2], "w+");
    char buf[100] = {0};
    int len = 0;
    int i = 0;
    int flag = 0;
    while(fgets(buf,100,fpS))
    {
        flag = 0;
        for (i = 0; i < sizeof(buf); i++)
        {
            if (buf[i]) flag = 1;
            if ((!buf[i]) && flag) break;
            fputc(buf[i],fpD);
        }
        
        memset(buf,0,100);
    }
    return 0;
}
