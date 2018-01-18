#include <iostream>
#include <vector>


#define VectorSize 1024

std::vector<int> v;

void Init(std::vector<int>& v, size_t size) {
    v.resize(size);
}


int main()
{
    pthread_t tid1, tid2;
    
    pthread_create(&tid1, NULL, consumer, NULL);
    pthread_create(&tid2, NULL, consumer, NULL);
    
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
