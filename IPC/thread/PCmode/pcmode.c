#ifndef __COMM_H__
#define __COMM_H__

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define CONSUMERCONT 2
#define PRODUCTERCONT 3

struct msg {
    struct msg* next;
    int num;
};

struct msg *head = NULL;

pthread_cond_t cond;
pthread_mutex_t mutex;
pthread_t threads[CONSUMERCONT + PRODUCTERCONT];

void* consumer(void* p)
{
    int num = *(int*)p;
    free(p);
    struct msg *mp;
    for (;;) {
        pthread_mutex_lock(&mutex);
        while ( head == NULL ) {
            printf("%d begin wait a condition\n", num);
            pthread_cond_wait(&cond, &mutex);
        }
        printf("%d end wait condition\n", num);
        printf("%d begin to consume %d\n", num, head->num);
        mp = head;
        head = mp->next;
        printf("%d end consume product\n", num);
        pthread_mutex_unlock(&mutex);
        free(mp);
        sleep(rand() % 5);
    }
}

void *producer(void *p)
{
    struct msg *mp;
    int num = *(int*)p;
    free(p);
    for (;;) {
        printf("%d begin produce product\n", num);
        mp = (struct msg*)malloc(sizeof(struct msg));
        mp->num = rand() % 1000 + 1;
        printf("produce %d\n", mp->num);
        pthread_mutex_lock(&mutex);
        mp->next = head;
        head = mp;
        printf("%d end produce product\n", num);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 5);
    }
}

int main()
{
    srand( time(NULL) );

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    int i;
    for (i = 0; i < PRODUCTERCONT; ++i) {
        int *p = (int*)malloc(sizeof(int));
        *p = i;
        pthread_create(&threads[i], NULL, producer, (void*)p);
    }

    for (; i < PRODUCTERCONT + CONSUMERCONT; ++i) {
        int *p = (int*)malloc(sizeof(int));
        *p = i;
        pthread_create(&threads[PRODUCTERCONT + i], NULL, consumer, (void*)p);
    }

    for (i = 0; i < CONSUMERCONT + PRODUCTERCONT; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

#endif
