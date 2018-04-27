#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
	1.接收用户的输入,并且输入特定字符串可以退出
	   (1).fgets() 获取输入
	   (2).strncmp() 用于比较用户是否输入了 "quit"
	2.执行用户输入的命令
	   (1).将命令字符串进行分割 "ls -l" ===> "ls" "-l"
	   (2).fork()一个子进程,用于替换
	   (3).使用exec函数执行命令

*/
#define DEBUG //定义DEBUG 标识
#ifdef DEBUG  //DEBUG 标识如果定义过,对应的代码就会编译,否则就不编译
   //代码	
#endif
int main(void)
{
	char buffer[64];
	char *param[5] = {NULL};
	pid_t pid;
	int i;
	while(1)
	{
		printf("minishell >");	
		fgets(buffer,sizeof(buffer),stdin);// '\n' '\0'
		//"ls -l\n\0"
		buffer[strlen(buffer) - 1] = '\0';//将'\n'去掉

		if (strncmp(buffer,"quit",4) == 0)
		{
			printf("Good Bye.\n");		
			break;
		}
		
		i = 0;
		param[0] = strtok(buffer," ");
#ifdef DEBUG
		printf("param[0] : %s\n",param[0]);
#endif
		while((param[++i] = strtok(NULL," ")) != NULL) //分割输入的字符串,并存入参数指针数组中
		{
#ifdef DEBUG
		printf("param[%d] : %s\n",i,param[i]);
#endif
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork()");	
			exit(-1);
		}
		else if (pid == 0)
		{
			if (execvp(param[0],param) < 0) //通过环境变量 path 来进行搜索可执行文件 env 查看环境变量
			{
				perror("execvp()");	
				exit(-1);
			}
		}
		else
		{
			wait(NULL);	//等待子进程退出,当子进程退出时,释放资源,就会返回
			//waitpid(-1,NULL,WNOHANG);
		}
	}
	return 0;
}
