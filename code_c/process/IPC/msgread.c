#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <error.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
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
    while(1)
    {

        msg_t msg1 = {0};
        if (-1 != msgrcv(msgid, &msg1, sizeof(msg_t)-sizeof(long), 1, IPC_NOWAIT))
        {
            printf("%ld:%s\n", msg1.mtype, msg1.msgbuf);
        }
         
        msg_t msg2 = {0};
        if (-1 != msgrcv(msgid, &msg2, sizeof(msg_t)-sizeof(long), 2, IPC_NOWAIT))
        {
            printf("%ld:%s\n", msg2.mtype, msg2.msgbuf);
        }

        sleep(1);
    }

       
    
    return 0;
}
