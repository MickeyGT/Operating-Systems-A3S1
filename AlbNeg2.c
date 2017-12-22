#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
#define BLACK 1
#define WHITE 0
#define INTERMEDIATE 2
int owner=WHITE,usingWhite=0,usingBlack=0,stopping=0,waitingWhite=0,waitingBlack=0;
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
    pthread_mutex_lock(&counter);
    if(color==WHITE)
        usingWhite--;
    else
        usingBlack--;
     pthread_mutex_unlock(&counter);
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
    printf("Thread %d with color %d is switching the owner color.\n",id,color);
    if(color==WHITE)
    {
        owner=INTERMEDIATE;
        stopping=1;
        while(usingWhite!=0){;}
        printf("All white are now done, giving control to black.\n");
        lastUseWhite=(int)time(NULL);
        sleep(1);
        pthread_cond_broadcast(&blackWait);
        owner=BLACK;
    }
    else
    {
        owner=INTERMEDIATE;
        stopping=1;
        while(usingBlack!=0){;}
        printf("All black are now done, giving control to white.\n");
        lastUseBlack=(int)time(NULL);
        owner=WHITE;
        sleep(1);
        pthread_cond_broadcast(&whiteWait);
        owner=WHITE;
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
    if(!(isOwner(color)))
    {
        printf("Thread %d with color %d is waiting\n",id,color);
        pthread_mutex_lock(&counter);
        if(color==WHITE)
            waitingWhite++;
        else
            waitingBlack++;
        pthread_mutex_unlock(&counter);
    }
    while(!(isOwner(color)))
    {
        pthread_mutex_lock(&ownerMutex);
        if(color==WHITE)
        {
            pthread_cond_wait(&whiteWait,&ownerMutex);
            waitingWhite--;
        }
        else
        {
            pthread_cond_wait(&blackWait,&ownerMutex);
            waitingBlack--;
        }
        pthread_mutex_unlock(&ownerMutex);    
    }
    if(color==WHITE)
    {
        if(colorStarving(BLACK))
        {
            usingWhite--;
            stopUsing(id,color);
        }
        else
        {
            useResource(id,color);
        }
    }
    else
    {
        if(colorStarving(WHITE))
        {
            usingBlack--;
            stopUsing(id,color);
        }
        else
        {
            useResource(id,color);
        }
    }
    return;
}

int main()
{
    pthread_t threads[100];
    lastUseBlack=(int)time(NULL);
    lastUseWhite=(int)time(NULL);
    for(int i=0;i<20;i++)
    {
        int * id = (int*)malloc (sizeof(int));
        *id=i;
        pthread_create(&threads[i],NULL,doRoutine,(void*)id);
        sleep(1);
    }
    sleep(20);
    return 0;
}




