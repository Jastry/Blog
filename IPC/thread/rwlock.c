#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct job {
    struct job *j_next;
    struct job *j_prev;
    int j_id; /* which thread handles this job*/
};

struct queue {
    struct job *q_head;
    struct job *q_tail;
    pthread_rwlock_t q_lock;
};

/*
 * Initialize a queue
 */
int queue_init(struct queue *qp);

/*
 * Insert a job at the head of the queue
 */
void job_insert(struct queue *qp, struct job *jp);

/*
 * Append a job on the tail of the queue
 */
void job_append(struct queue *qp, struct job *jp);

/*
 * Remove the given job from queue
 */
void job_remove(struct queue *qp, struct job *jp);

/*
 * Find a job for the given thread ID.
 */
struct job * job_find(struct queue *qp, struct job *id);


struct queue q;
struct job jobs[10];



void *finder(void *arg) 
{
    struct job *jp = (struct job*)arg;
    printf("id is %lu thread finding %d\n", pthread_self(), jp->j_id);
    while (1) {
        if ( NULL != job_find(&q, jp) ){
            printf("find %d\n", jp->j_id);
            break;
        }
        else
            sleep(1);
    }
}

void *producter1(void *arg) 
{
    int i = 0;
    for (; i < 5; ++i) {
        printf("id is %lu productering %d\n", pthread_self(), i);
        job_insert(&q, &jobs[i]);
        sleep(1);
    }

    for (; i < 10; ++i) {
        printf("id is %lu productering %d\n", pthread_self(), i);
        job_append(&q, &jobs[i]);
        sleep(1);
    }
}


int main()
{
       
    pthread_t tid1, tid2, tid3;
    for (int i = 0; i < 10; ++i) {
        jobs[i].j_id = i;
        jobs[i].j_next = NULL;
        jobs[i].j_prev = NULL;
    }
    queue_init(&q);
    pthread_create(&tid1, NULL, producter1, NULL);
    pthread_create(&tid2, NULL, finder, (void* )&jobs[4]);
    pthread_create(&tid3, NULL, finder, (void* )&jobs[9]);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    return 0;
}


int queue_init(struct queue *qp)
{
    int err;

    qp->q_head = NULL;
    qp->q_tail = NULL;
    err = pthread_rwlock_init(&qp->q_lock, NULL);
    if (err != 0) {
        return err;
    }
    /* continue initialization ...*/
    return 0;
}

void job_insert(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = qp->q_head;
    jp->j_prev = NULL;
    if (qp->q_head != NULL)
        qp->q_head->j_prev = jp;
    else 
        qp->q_tail = jp;    /* list was empty */
    qp->q_head = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

void job_append(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    jp->j_next = NULL;
    jp->j_prev = qp->q_tail;
    if (qp->q_tail != NULL) 
        qp->q_tail->j_next = jp;
    else 
        qp->q_head = jp;    /* list was empty */
    qp->q_tail = jp;
    pthread_rwlock_unlock(&qp->q_lock);
}

void job_remove(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if (jp == qp->q_head) {
        qp->q_head = jp->j_next;
        if (qp->q_tail == jp)
            qp->q_tail = NULL;
        else 
            jp->j_next->j_prev = jp->j_prev;
    } else if (jp == qp->q_tail) {
        qp->q_tail = jp->j_prev;
        jp->j_prev->j_next = jp->j_next;
    } else {
        jp->j_prev->j_next = jp->j_next;
        jp->j_next->j_prev = jp->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}


struct job* job_find(struct queue *qp, struct job *id)
{
    struct job* jp;
    if ( pthread_rwlock_rdlock(&qp->q_lock) != 0)
        return NULL;
    
    for (jp = qp->q_head; jp != NULL; jp = jp->j_next) {
        if ( pthread_equal(jp->j_id, id->j_id) ) 
            break;
    }
    pthread_rwlock_unlock(&qp->q_lock);
    return (jp);
}
