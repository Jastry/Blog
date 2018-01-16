#include "comm.h"


int main()
{
    int shmid = GetShm(4096);
    sleep(1);
    char *addr = shmat(shmid, NULL, 0);
    sleep(2);
    int i = 0;
    for (; i < 26; ++i) {
        addr[i] = 'a' + i;
        addr[i + 1] = 0;
        sleep(1);
    }
    shmdt(addr);
    sleep(1);
    return 0;
}
