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
    msg_t msg1 = {0};
    msg1.mtype = 1;
    strcpy(msg1.msgbuf, "hello");
    int ret =  msgsnd(msgid, &msg1, sizeof(msg_t) - sizeof(long), 0);
    if (-1 == ret)
    {
        perror("msgsnd");
        return 0;
    }

    
    
    return 0;
}
