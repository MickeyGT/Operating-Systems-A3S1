#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
#define BLACK 1
#define WHITE 0
int owner=WHITE,usingWhite=0,usingBlack=0;
int lastUseBlack,lastUseWhite;
pthread_mutex_t counter = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t whiteWait = PTHREAD_COND_INITIALIZER;
pthread_cond_t blackWait = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ownerMutex = PTHREAD_MUTEX_INITIALIZER;

void useResource(int id, int color)
{
    printf("Thread %d with color %d is using the resource.\n",id,color);
    pthread_mutex_lock(&counter);
    if(color==WHITE)
        usingWhite++;
    else
        usingBlack++;
    
    pthread_mutex_unlock(&counter);
    sleep(2);
    stopUsing(id,color);
}

int colorStarving(int color)
{
    int otherColor;
    if(color==WHITE)
    {
        //printf("Time after switch:%d ",(int)time(NULL)-lastUseBlack);
        if((int)time(NULL)-lastUseWhite<=5)
            return 0;
        else
            return 1;
    }
    else
    {
        //printf("Time after switch:%d ",(int)time(NULL)-lastUseWhite);
        if((int)time(NULL)-lastUseBlack<=5)
            return 0;
        else
            return 1;
    }
}

void stopUsing(int id,int color)
{
    if(color==WHITE)
    {
        pthread_mutex_lock(&counter);
        usingWhite--;
        pthread_mutex_unlock(&counter);
        while(usingWhite!=0){;}
        printf("All white are now done, giving control to black.\n");
        sleep(1);
        pthread_cond_broadcast(&blackWait);
    }
    else
    {
        pthread_mutex_lock(&counter);
        usingBlack--;
        pthread_mutex_unlock(&counter);
        while(usingBlack!=0){;}
        printf("All black are now done, giving control to white.\n");
        owner=WHITE;
        sleep(1);
        pthread_cond_broadcast(&whiteWait);
    }
}

int isOwner(int myColor)
{
    return myColor==owner;
}

void* doRoutine(void *arg)
{
    int id,color;
    srand(time(NULL));
    color = rand()%2;
    id = *((int*)arg);
    free(arg);
    while(!(isOwner(color)))
    {
        printf("Thread %d with color %d is waiting\n",id,color);
        pthread_mutex_lock(&ownerMutex);
        if(color==WHITE)
            pthread_cond_wait(&whiteWait,&ownerMutex);
        else
            pthread_cond_wait(&blackWait,&ownerMutex);
        pthread_mutex_unlock(&ownerMutex);    
    }
    if(color==WHITE)
    {
        if(colorStarving(BLACK))
        {
            owner=BLACK;
            stopUsing(id,color);
        }
        else
        {
            useResource(id,color);
             pthread_mutex_lock(&counter);
            usingWhite--;
            pthread_mutex_unlock(&counter);
        }
    }
    else
    {
        if(colorStarving(WHITE))
        {
            owner=WHITE;
            stopUsing(id,color);
        }
        else
        {
            useResource(id,color);
            pthread_mutex_lock(&counter);
            usingBlack--;
            pthread_mutex_unlock(&counter);
        }
    }
   
    return;
}

int main()
{
    pthread_t threads[100];
    lastUseBlack=(int)time(NULL);
    for(int i=0;i<100;i++)
    {
        int * id = (int*)malloc (sizeof(int));
        *id=i;
        pthread_create(&threads[i],NULL,doRoutine,(void*)id);
        sleep(1);
    }
    sleep(20);
    return 0;
}
