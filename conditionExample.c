#include<pthread.h>
#include<unistd.h>
#include<stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int aux = 1;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;


void* threadLockTest(void* arg) {
   printf("Child wants to take the mutex\n");
   
   pthread_mutex_lock(&mutex);
   
   printf("Child took the mutex\n");
   sleep(7);
   if(aux == 1) {
       printf("Child before wait\n");
       
       pthread_cond_wait(&condition, &mutex);
       printf("Child after wait\n");
   }
   sleep(5); 
   
   pthread_mutex_unlock(&mutex);
   
   printf("Child left the mutex\n");
}

int main() {

    pthread_t threadID;
    pthread_create(&threadID, NULL, threadLockTest, NULL);
    sleep(5);
    printf("Main wants to take the mutex\n");
   
    pthread_mutex_lock(&mutex);
   
    printf("Main took the mutex\n");
   
    if(aux == 1) {
        printf("Main before signal\n");
        pthread_cond_signal(&condition);
        printf("Main after signal\n");
            
    }
   
    sleep(5); 
   
    pthread_mutex_unlock(&mutex);
   
   printf("Main left the mutex\n");
     
     pthread_join(threadID, NULL);   

     return 0;
}