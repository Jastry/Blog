//#pragma once
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define PATHNAME "."
#define PROJ_ID 0x233

union semun {              
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */          
    struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */           
};

int CreateSem(int nsems); 

int GetSem();

int InitSem(int semid, int nums, int init_val);

int GetSemValue(int semid, int who, int *init_val);

int P(int semid, int who);

int V(int semid, int who);

int DestroySem(int semid);

#endif //__COMMON_H__


