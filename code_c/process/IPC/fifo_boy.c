#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[])
{
    char buf [1024] = {0};
   
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        exit(-1);
    }
    else if (0 == pid)
    {
        //子进程
        int fd2 = 0;
		if (mkfifo("fifo2",0777) < 0 && errno != EEXIST)
    	{
        	perror("mkinfo");
        	exit(-1);
    	}
    	fd2 = open("fifo2", O_RDONLY);
    	if (fd2 < 0)
    	{
        	perror("open");
        	exit(-1);
    	}
    	
        while(1)
        {
            read(fd2, buf, 1024);
            printf("girl:\n");
            printf("%s\n", buf);
            usleep(200);
        }
		close(fd2);
        exit(0);
    }
	
	if (mkfifo("fifo1",0777) < 0 && errno != EEXIST)
    {
        perror("mkinfo");
        exit(-1);
    }
    
    int fd1 = 0;
    fd1 = open("fifo1", O_WRONLY);
    if (fd1 < 0)
    {
        perror("open");
        exit(-1);
    }
    //父进程
    while(1)
    {
        gets(buf);
        write(fd1, buf, 1024);
        printf("boy:\n%s\n", buf);
        usleep(200);
    }
    close(fd1);
    return 0;
}
