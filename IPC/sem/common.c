#include "common.h"

static int CommonSem(int nsems, int flags)
{
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key < 0) {
        perror("ftok");
        return -1;
    }
    int semid = semget(key, nsems, flags);
    if (semid < 0) {
        perror("semget");
        return -1;
    }
    return semid;
}

int CreateSem(int nsems) 
{
    return CommonSem(nsems, IPC_CREAT | IPC_EXCL | 0666);
}

int InitSem(int semid, int nums, int init_val)
{
    union semun _semun;
    _semun.val = init_val;
    int ret = semctl(semid, nums, SETVAL, _semun);
    if (ret < 0) {
        perror("semctl SETVAL");
        return -1;
    }
    return 0;
}

int GetSemValue(int semid, int who, int *init_val)
{
    int ret = semctl(semid, who, GETVAL);
    if (ret < 0) {
        perror("semctl GETVAL");
        return -1;
    }
    *init_val = ret;
    return 0;
}

int GetSem()
{
    return CommonSem(0, IPC_CREAT);
}

static int CommonOption(int semid, int who, int option)
{
    struct sembuf _sembuf;
    _sembuf.sem_num = who;
    _sembuf.sem_op = option;
    _sembuf.sem_flg = 0;
    int ret = semop(semid, &_sembuf, 1);
    if (ret < 0) {
        perror("semop");
        return -1;
    }
    return 0;
}

int P(int semid, int who)
{
    return CommonOption(semid, who, -1);
}

int V(int semid, int who)
{
    return CommonOption(semid, who, 1);
}

int DestroySem(int semid)
{
    int ret = semctl(semid, 0, IPC_RMID);
    if (ret < 0) {
        perror("semctl");
        return -1;
    }
    return 0;
}
