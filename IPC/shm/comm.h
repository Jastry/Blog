#ifndef __COMM_H__
#define __COMM_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define PATHNAME  "."
#define PROJ_ID  0x233

int CreateShm(int size);
int DestroyShm(int shmid);
int GetShm(int size);



#endif //__COMM_H__
