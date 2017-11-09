#include<pthread.h>
#include<unistd.h>

int MAX_SIZE = 4;
Item itemBuffer[MAX_SIZE];
void producer() {

while(true) {
    Item item = produce();
    insert(item);
}

}

void consumer() {

    while(true) {
        Item item = remove();
        consume(item);
    }

}

void insert(Item item) {
    pthread_mutex_lock(&bufferMutex);
    while(bufferIsFull()) {
        pthread_cond_wait(&prodCond, &bufferMutex);
    }    
    
    if(bufferIsEmpty()) {
        pthread_cond_signal(&consCond);
            
        
    }
    itemBuffer[lastItem] = item;
    
    lastItem++;
    pthread_mutex_unlock(bufferMutex);
}


Item remove() {
    
    
    pthread_mutex_lock(&bufferMutex);
    
    while(bufferIsEmpty() {
        pthread_cond_wait(&consCond, &bufferMutex);
    }
    Item item = itemBuffer[lastItem];
    if(bufferIsFull() ){
        pthread_cond_signal(&prodCond);
    }
    
    lastItem --;
    pthread_mutex_unlock(&bufferMutex);
    return item;
    

}