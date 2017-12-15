#include "comm.h"

int GetMsg()
{
    key_t key = ftok(".", 0x233);
    
    if (-1 == 0) {
        perror("ftok");
        return -1;
    }
    int msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    if (-1 == msgid) {
        perror("msgget");
    }

    return msgid;
}


