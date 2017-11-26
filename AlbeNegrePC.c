#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
#define BLACK 1
#define WHITE 0
int owner=WHITE,waitingWhite=0,waitingBlack=0,usingWhite=0,usingBlack=0;
int lastUseBlack,lastUseWhite;
pthread_mutex_t counter = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t whiteWait = PTHREAD_COND_INITIALIZER;
pthread_cond_t blackWait = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ownerMutex = PTHREAD_MUTEX_INITIALIZER;

void useResource(int id, int color)
{
    printf("Thread %d with color %d is using the resource.\n",id,color);
    sleep(1);
}

int otherColorNotStarving(int color)
{
    if(color==WHITE)
    {
        if((int)time(NULL)-lastUseBlack<=5)
            return 0;
        else
            return 1;
    }
    else
    {
        if((int)time(NULL)-lastUseWhite<=5)
            return 0;
        else
            return 1;
    }
}

void stopUsingWait(int id,int color)
{
    pthread_mutex_lock(&counter);
    if(color==WHITE)
    {
        waitingWhite++;
        usingWhite--;
        printf("Thread %d is now giving his place.\n",id);
        if(usingWhite==0)
        {
            printf("All white are now done, giving control to black.\n");
            pthread_cond_signal(&blackWait);
        }
    }
    else
    {
        waitingBlack++;
        usingBlack++;
        printf("Thread %d is now giving his place.\n",id);
        if(usingBlack==0)
        {
            printf("All black are now done, giving control to white.\n");
            pthread_cond_signal(&whiteWait);
        }
    }
    
    pthread_mutex_unlock(&counter);
}

int isOwner(int myColor)
{
    return myColor==owner;
}

void* doRoutine(void *arg)
{
    int id,color;
    //srand(time(NULL));
    //color = rand()%2;
    id = *((int*)arg);
    if(id<5)
        color=WHITE;
    else
        color=BLACK;
    free(arg);
    while(1)
    {
        pthread_mutex_lock(&ownerMutex);
        printf("Thread %d is using the mutex.",id);
        if(isOwner(color)&&otherColorNotStarving(color))
            useResource(id,color);
        else
            if(isOwner(color))
                stopUsingWait(id,color);
            else
            {
                if(color==WHITE)
                    pthread_cond_wait(&whiteWait,&ownerMutex);
                else
                    pthread_cond_wait(&blackWait,&ownerMutex);
            }
        pthread_mutex_unlock(&ownerMutex);    
    }
}

int main()
{
    pthread_t threads[100];
    for(int i=0;i<10;i++)
    {
        int * id = (int*)malloc (sizeof(int));
        *id=i;
        pthread_create(&threads[i],NULL,doRoutine,(void*)id);
    }
    return 0;
}
