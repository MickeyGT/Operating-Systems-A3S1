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



About
FAQ
Blog
Terms of Use
Contact Us
GDB Tutorial
Online Java/Python Debugger
2017 © GDB Online
    Run   Debug   Stop   Share   Save  { } Beautify  Language      
main.c

  
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
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
input

Child wants to take the mutex                                                                                                                                                                                 
Child took the mutex                                                                                                                                                                                          
Main wants to take the mutex                                                                                                                                                                                  
Child before wait                                                                                                                                                                                             
Main took the mutex                                                                                                                                                                                           
Main before signal                                                                                                                                                                                            
Main after signal                                                                                                                                                                                             
Main left the mutex                                                                                                                                                                                           
Child after wait                                                                                                                                                                                              
Child left the mutex
