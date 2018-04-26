#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main(int argc, const char *argv[])
{
    int ret = 0;
    struct stat buf = {0};
    char *mon[12] = {"Jan", "Feb", "Mar", "Apr", 
                     "May", "Jun", "Jul", "Aug",
                     "Sep", "Oct", "Nov", "Dec"};
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s %s\n", argv[0], argv[1]);
        return 0;
    }

    //��ȡ�ļ������������Ϣ
    ret = stat(argv[1], &buf);
    if (-1  == ret)
    {
        perror("stat");
        return 0;
    }

    //��ӡ�ļ�����
    if (S_ISREG(buf.st_mode))
        putchar('-');
    if (buf.st_mode & S_IFDIR)
        putchar('d');
    if ((buf.st_mode & S_IFMT) == S_IFLNK)
        putchar('l');
    //��ӡ�ļ���Ȩ����Ϣ
    if (buf.st_mode & S_IRUSR)
    {
        putchar('r');
    }
    else
    {
        putchar('-');
    }
    if (buf.st_mode & S_IWUSR)
    {
        putchar('w');
    }
    else
    {
        putchar('-');
    }
    if (buf.st_mode & S_IXUSR)
    {
        putchar('x');
    }
    else
    {
        putchar('-');
    }
    if (buf.st_mode & S_IRGRP)
    {
        putchar('r');
    }
    else
    {
        putchar('-');
    }
    if (buf.st_mode & S_IWGRP)
    {
        putchar('w');
    }
    else
    {
        putchar('-');
    }
    if (buf.st_mode & S_IXGRP)
    {
        putchar('x');
    }
    else
    {
        putchar('-');
    }
    if (buf.st_mode & S_IROTH)
    {
        putchar('r');
    }
    else
    {
        putchar('-');
    }
    if (buf.st_mode & S_IWOTH)
    {
        putchar('w');
    }
    else
    {
        putchar('-');
    }
    if (buf.st_mode & S_IXOTH)
    {
        putchar('x');
    }
    else
    {
        putchar('-');
    }
    
    putchar(' ');

    //��ӡ�ļ���Ӳ������
    printf("%d", buf.st_nlink);
    //��ӡ�ļ��������û�
    printf(" %s", getpwuid(buf.st_uid)->pw_name);
    //��ӡ�ļ�������
    printf(" %s", getgrgid(buf.st_gid)->gr_name);
    //��ӡ�ļ���С
    printf(" %ld", buf.st_size);
    //��ӡ�ļ�����޸�ʱ��
    struct tm *tp = localtime(&buf.st_mtime);
    printf(" %s %d %02d:%02d", mon[tp->tm_mon], tp->tm_mday, tp->tm_hour, tp->tm_min);

    printf(" %s", argv[1]);
    putchar('\n');
    return 0;
}
