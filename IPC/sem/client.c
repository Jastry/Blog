#include "common.h"
#include <unistd.h>

int main()
{

    int init_val = 1;
    int nsems = 1;
    int index_of_sem = 0;
    int semid = CreateSem(nsems);  //nsms 个信号量
    int ret = -1;
    InitSem(semid, index_of_sem, init_val);   //初始化第 index_of_sem 个为 init_val
    char ch = 'a';
    while (ch <= 'z') {
        P(semid, 0);
        write(1, &ch, 1);
        ch++;
        sleep(1);
        V(semid, 0);
    }
    DestroySem(semid);
    return 0;
}
