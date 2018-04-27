#include <stdio.h>
#include<sys/types.h>
#include<dirent.h>

int main(int argc, const char *argv[])
{
    DIR *dir = NULL;
    dir = opendir("1");
    if (NULL == dir)
    {
        perror("opendir");
        return 0;
    }
    struct dirent *pdirent = NULL;
    int regnum = 0;
    int dirnum = 0;

    while(pdirent = readdir(dir))
    {
        //printf("d_ino = %ld, d_off = %ld, ", pdirent->d_ino, pdirent->d_off);
        //printf("d_reclen = %d, d_type = %d, name = %s\n ", pdirent->d_reclen, pdirent->d_type, pdirent->d_name);
        if (pdirent->d_type  == DT_REG)
            regnum++;
        if (pdirent->d_type  == DT_DIR)
            dirnum++;

    }
    printf("reg file num :%d\n", regnum);
    printf("dir file num :%d\n", dirnum-2);
    closedir(dir);
    return 0;
}
