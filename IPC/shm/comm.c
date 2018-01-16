#include "comm.h"

static int CommShm(int size, int flag) 
{
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key < 0) {
        perror("ftok");
        return -1;
    }
    
    int shmid = shmget(key, size, flag);
    if (shmid == -1) {
        perror("shmget");
        return -2;
    }
    return shmid;
}

int DestroyShm(int shmid)
{
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl RMID");
        return -1;
    }
}

int CreateShm(int size)
{
    return CommShm(size, IPC_CREAT | IPC_EXCL | 0666);
}

int GetShm(int size)
{
    return CommShm(size, IPC_EXCL | 0666);
}
