#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

int g_val = 0;

int main()
{

    return 0;
}






#if 0
int g_val = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* run(void* arg) {
    for (size_t i = 0; i < 50000; ++i) {
        pthread_mutex_lock(&lock);
        int tamp = g_val;
        tamp = tamp + 1;
        g_val = tamp;
        pthread_mutex_unlock(&lock);
        printf("p_thread id is %lu, g_val = %d\n",pthread_self(),  g_val);
    }
}

int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, run, NULL);
    pthread_create(&tid2, NULL, run, NULL);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
#endif

#if 0
typedef struct ListNode{
	int data;
	struct ListNode* next;
} Node;

Node* Create(int value) {
	Node* node = (Node*)malloc(sizeof (Node));
	node->data = value;
	node->next = NULL;
}

void Push(Node* head, int value)
{
	Node* node = Create(value);
	node->next = head->next;
	head->next = node;
}

void Destroy(Node* node)
{
	if (node != NULL) {
		free(node);
	}
}

void pop(Node* head, int *value) {
	if (head->next == NULL) {
		return;
	}
	Node* del = head->next;
	Node* next = del->next;
	head->next = next;
	*value = del->data;
	Destroy(del);
}


void Init(Node** head) {
	*head = Create(0);
}

pthread_cond_t g_cond;
pthread_mutex_t g_mutex;
Node* g_head;

void* producter(void* arg) {
	size_t count = 0;
	while (1) {
		Push(g_head, count++);
		printf("producter %lu\n", count);
		//usleep(823455);
	}
}

void* consumer(void* arg) {
	int value = 0;
	while (1) {
		pop(g_head, &value);
		//usleep(521332);
		printf("consumer %d\n", value);
	}	
}


int main(int argc, char const *argv[])
{
    Init(&g_head);
	pthread_cond_init(&g_cond, NULL);
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, producter, NULL);
	pthread_create(&tid2, NULL, consumer, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_cond_destroy(&g_cond);
	return 0;
}
#endif

#if 0
void* threadEntery1(void* arg) {
	while (1){
		usleep(423456);
		printf("%s\n", "传球");
	}
}

void* threadEntery2(void* arg) {
	while (1){
		usleep(123456);
		printf("%s\n", "投篮");
	}
}
#endif

#if 0
pthread_cond_t g_cond;
pthread_mutex_t g_mutex;

void* threadEntery1(void* arg) {
	while (1){
		usleep(423456);
		printf("%s\n", "传球");
		pthread_cond_signal(&g_cond);
	}
}

void* threadEntery2(void* arg) {
	while (1){
		usleep(123456);
		pthread_cond_wait(&g_cond, &g_mutex);
		printf("%s\n", "投篮");
	}
}


int main(int argc, char const *argv[])
{
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, threadEntery1, NULL);
	pthread_create(&tid2, NULL, threadEntery2, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}
#endif

#if 0
void* threadEntery(void* arg) {
    while (1){
        sleep(1);
                printf("%s\n", "other thread");
                            
    }
                        return NULL;


}

int main(int argc, char const *argv[])
{
        pthread_t tid;
            int ret = pthread_create(&tid, NULL, threadEntery, NULL);
            if (ret != 0) {
                        perror("pthread_create");
                                return 1;
                                    
            }
                ret = pthread_detach(tid);
                if (ret != 0) {
                            perror("pthread_detach");
                                    return 1;
                                        
                }

                while (1) {
                            //main thread
                                  printf("%s\n", "main thread");
                                          sleep(1);
                                              
                } 
                    
                    return 0;

}

#endif
