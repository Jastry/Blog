#ifndef __COMMON_H__
#define __COMMON_H__

#include <errno.h>
#include <limits.h>
#include <unistd.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

/*
 * If OPEN_MAX is indeterminate, this might be inadquate
 */
#define OPEN_MAX_GUESS 256

long open_max(void)
{
    if (openmax == 0) { /* first time throug*/
        errno = 0;
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
            if (errno == 0) {
                openmax = OPEN_MAX_GUESS;   /*不确定的*/
            } else {
                perror("sysconf");
            }
        }
    }
    return (openmax);
}

#endif  //__COMMON_H__
