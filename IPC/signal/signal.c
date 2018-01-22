#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int quitflag;   /* 通过线程设置为非0 */
sigset_t mask;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;

void *thread_fun(void *arg)
{
    int err, signo;
    for ( ; ; ) {
        err = sigwait( &mask, &signo);
        if ( err != 0 ) {
            perror( "sigwait" );
        }
        switch (signo) {
            case SIGINT:
                printf("\ninterrupt\n");
                break;
            case SIGQUIT:
                pthread_mutex_lock( &lock );
                quitflag = 1;
                pthread_mutex_unlock( &lock );
                pthread_cond_signal( &waitloc );
                return 0;
            default:
                printf("unexcept signal %d\n", signo);
                exit(0);
        }
    }
}

int main()
{
    int err;
    sigset_t oldmask;
    pthread_t tid;

    sigemptyset( &mask );
    sigaddset( &mask, SIGINT );
    sigaddset( &mask, SIGQUIT );
    if ( (err = pthread_sigmask( SIG_BLOCK, &mask, &oldmask )) != 0 ) { 
        perror("pthread_sigmask");
        exit(1);
    }

    err = pthread_create( &tid, NULL, thread_fun, NULL );
    if ( err != 0 ) {
        perror("pthread_create");
        exit(2);
    }

    pthread_mutex_lock( &lock );
    while ( quitflag != 0 ) {
        pthread_cond_wait( &waitloc, &lock );
    }
    pthread_mutex_unlock( &lock );

    /* SIGQUIT has been caught and is now blocked do whatever */

    quitflag = 0;
    if ( sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0 ) {
        perror("sigprocmask");
        exit(3);
    }
    pause();
}

