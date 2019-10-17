#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <error.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <time.h>
typedef struct 
{
    long mtype;
    char msgbuf[1024];
}msg_t;

int main(int argc, const char *argv[])
{
    key_t key = ftok("1.txt", 'b');
    if (-1 == key)
    {
        perror("ftok");
        return 0;
    }

    int msgid = msgget(key, IPC_CREAT|0666);
    if (-1 == msgid)
    {
        perror("msgget");
        return 0;
    }
    int num = 0;
    time_t tim = 0;
    struct tm *ptm = NULL;
    int n = 0;
    while(1)
    {
        if (num %1 == 0)
        { 
            msg_t msg1 = {0};
            msg1.mtype = 1;
            tim = time(NULL);
            ptm = localtime(&tim);
            sprintf(msg1.msgbuf, "%d-%d-%d %d:%d:%d", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, 
                ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
            int ret =  msgsnd(msgid, &msg1, sizeof(msg_t) - sizeof(long), 0);
            if (-1 == ret)
            {
                perror("msgsnd");
                return 0;
            }
        }

        if (num %5 == 0)
        {
            msg_t msg2 = {0};
            msg2.mtype = 2;
        
            sprintf(msg2.msgbuf, "%d", n++);
            int ret =  msgsnd(msgid, &msg2, sizeof(msg_t) - sizeof(long), 0);
            if (-1 == ret)
            {
                perror("msgsnd");
                return 0;
            }
        }
        num++;
        sleep(1);
    }

       
    
    return 0;
}
