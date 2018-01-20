#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "locker.h"

locker lock;

/*
 * 子线程运行的函数，他首先获得互斥锁，然后暂停 5s，再释放该互斥锁 
 */
void *anther(void *arg) 
{
    printf("in child thread, lock the mutex\n");
    lock.lock();
    sleep(3);
    lock.unlock();
}

void prepare()
{
    lock.lock();
}

void parent()
{
    lock.unlock();
}

void child()
{
    lock.unlock();
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, anther, NULL);
   
    /*
     * 父进程中的主线成现暂停 1s 确保在执行 fork 之前， 
     * 子线程已经开始运行并且拿到了互斥锁 
     */

    sleep(1);
    //pthread_atfork(prepare, parent, child);
    lock.lock();
    lock.unlock();
    int pid = fork();
    if ( pid < 0 ) {
        pthread_join( pid, NULL );
        return 1;
    } else if ( 0 == pid ) {
        printf(" I'm child, want to get the lock\n");
        /*
         * 子进程从父进程中继承了互斥锁的状态，
         * 该互斥锁处于锁住状态，这是由父进程中的子线程引起的，
         * 因此子进程想要获取锁就会一直阻塞，尽管从逻辑上他不应该被阻塞
         */
        sleep(1);
        lock.lock();
        printf("I am child  get lock \n");
        sleep(2);
        lock.unlock();
        std::cout << "I'm child I relese the lock\n";
        exit(0);
    } else {
        std::cout << "I'm father I want get lock\n";
        lock.lock();
        std::cout << "I'm father I get lock\n";

        sleep(3);
        std::cout << "I'm father I relese the lock\n";
        lock.unlock();
        wait(NULL);
    }

    pthread_join(tid, NULL);
    return 0;
}
