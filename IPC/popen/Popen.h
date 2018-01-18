#ifndef __POPEN_H__
#define __POPEN_H__

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "comm.h"

static pid_t *childpid = NULL;

static int maxfd;


FILE* Popen(const char* cmdstring, const char* type)
{
    int i;
    int fd[2];
    FILE *fp;
    pid_t pid;

    /*only allow "r" or "w"*/
    if ((type[0] != 'r' && type[0] != 'w' || type[1] != 0)) {
        errno = EINVAL; /*required by POSIX*/
        return NULL;
    }
    if (childpid == NULL) { //first time through
        /* allocate zeroed out array for child pids*/
        /*为孩子的pids分配0的数组*/
        maxfd = open_max();
        if ((childpid = calloc(maxfd, sizeof (pid_t))) == NULL)
            return NULL;
    }
    if (pipe(fd) < 0) {
        perror("pipe");
        return NULL;
    }
    if ((pid = fork()) < 0) {
        return NULL;
    } else if (0 == pid) {
        if (*type == 'r') {
            close(fd[0]);
            if (fd[1] != 1) {
                dup2(fd[1], 1);
                close(fd[1]);
            }
        } else {
            close(fd[1]);
            if (fd[0] != 0) {
                dup2(fd[0], 0);
                close(fd[0]);
            }
        }
        /*close all descriptors in childpid[]*/
        for (i = 0; i < maxfd; ++i) {
            if (childpid[i] > 0)
                close(i);
        }
        execl("/bin/sh", "sh", "-c", cmdstring, (char*)0);
        _exit(127);
    }
    if (*type == 'r') {
        close(fd[1]);
        if ((fp = fdopen(fd[0], type)) == NULL)
            return NULL;
    } else {
        close(fd[0]);
        if ((fp = fdopen(fd[1], type)) == NULL)
            return NULL;
    }
    childpid[fileno(fp)] = pid; /*remember child pid for this fd*/
    return (fp);
}

int Pclose(FILE* fp)
{
    int fd, stat;
    pid_t pid;

    if (childpid == NULL) {
        errno = EINVAL;
        return (-1);
    }

    fd = fileno(fp);
    if ((pid = childpid[fd]) == 0) {
        errno = EINVAL;
        return (-1);
    }

    childpid[fd] = 0;
    if (fclose(fp) == EOF) {
        return (-1);
    }

    while (waitpid(pid, &stat, 0) < 0) {
        if (errno != EINTR) {
            return (-1);
        }
    }
    return (stat);
}

#endif
