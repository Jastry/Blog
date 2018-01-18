#include "common.h"

int main()
{
    int semid = GetSem();
    char ch = 'b';
    while (ch <= 'z') {
        P(semid, 0);
        write(1, &ch, 1);
        ch++;
        sleep(1);
        V(semid, 0);
    }
    return 0;
}
