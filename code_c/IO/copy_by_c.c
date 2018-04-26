#include <stdio.h>


int main(int argc, const char *argv[])
{
    FILE *fpS = NULL;
    FILE *fpD = NULL;
    int ch = 0;

    if (argc < 3)
    {
        fprintf(stderr, "para error:%s srcfile desfile\n", argv[0]);
        return 0;
    }

    fpS = fopen(argv[1], "r");
    if (NULL == fpS)
    {
        perror("open src");
        return 0;
    }
    fpD = fopen(argv[2], "w+");
    if (NULL == fpD)
    {
        perror("open des");
        return;
    }
    
    while(EOF != (ch = fgetc(fpS)))
    {
        fputc(ch, fpD);
    }
    fclose(fpS);
    fclose(fpD);

    return 0;
}
