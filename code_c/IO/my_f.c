#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
int my_fgetc(int fd)
{
    int ch = -1;
    int n = read(fd,&ch,1);
    if (0 == n)
    {
        return EOF;
    }

    return ch;
}

char * my_fgets(char *s,int size,int fd)
{
    char *ps = NULL;
    ps = (char *)malloc(size);
    int i;
    if (NULL == ps)
    {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }
    memset(ps,0,size);
    int n = read(fd, ps, size-1);
    if (0 == n)
    {
        return NULL;
    }

    printf("ps = %s", ps);
    //1.解决按照\n结束的问题
    for (i = 0; i < n-1; i++)
    {
        if (ps[i] == '\n')
        {
            break;
        }
    }
    ps[i+1] = '\0';
    //2.解决偏移量回到刚刚的位置
    lseek(fd,-(n - i-1) ,SEEK_CUR);
    memcpy(s,ps,i+1+1);
    return s;
}
int my_fputc(int c, int fd)
{
    return write(fd, &c, 1);
}
int my_fputs(char *s, int fd)
{
    int i = 0;
    int size = 0;
    
    for (i = 0; i < strlen(s); i++)
    {
        if (s[i] == '\0')
            break;
        size++;
    }
    write(fd,s,size);
    return 1;
}
int main(int argc, const char *argv[])
{
    int fd = -1;

    fd = open("1.txt", O_RDWR|O_APPEND);
    if(-1 == fd)
    {
        perror("open");
        return 0;
    }

   // int ch = my_fgetc(fd);
    //printf("ch = %c\n", ch);
    //ch = my_fgetc(fd);
    //printf("ch = %c\n", ch);

#if 0

    char buf[10] = {0};
    my_fgets(buf,10,fd);
    printf("buf = %s\n", buf);
    my_fgets(buf,10,fd);
    printf("buf = %s\n", buf);
#endif
    //my_fputc('a', fd);
    //my_fputc('a', fd);
    char buf[] = {'a','b','\n','c','\0','d'};
    my_fputs(buf,fd);
    return 0;
}
