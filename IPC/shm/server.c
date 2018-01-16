#include "comm.h"

int main()
{
    int shmid = CreateShm(4096);
   
    char* shmaddr = shmat(shmid, NULL, 0);
    sleep(2);
    int i = 0;
    for (; i < 26; ++i) {
        printf("client : %s\n", shmaddr);
        sleep(1);
    }
    shmdt(shmaddr);
    sleep(2);
    DestroyShm(shmid);
    return 0;
}
