#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
#define BLACK 1
#define WHITE 0
int owner=WHITE,waitingWhite=0,waitingBlack=0,usingWhite=0,usingBlack=0;
int colorUsing[10]={0,0,0,0,0,0,0,0,0,0};
int lastUseBlack,lastUseWhite;
pthread_mutex_t counter = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t whiteWait = PTHREAD_COND_INITIALIZER;
pthread_cond_t blackWait = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ownerMutex = PTHREAD_MUTEX_INITIALIZER;

void useResource(int id, int color)
{
    if(colorUsing[id]==0)
    {
        colorUsing[id]=1;
        if(color==WHITE)
        {
            pthread_mutex_lock(&counter);
            usingWhite++;
            waitingWhite--;
            pthread_mutex_unlock(&counter);
        }
        else
        {
            pthread_mutex_lock(&counter);
            usingBlack++;
            waitingBlack--;
            pthread_mutex_unlock(&counter);
        }
    }
    if(color==WHITE)
        lastUseWhite=(int)time(NULL);
    else
        lastUseBlack=(int)time(NULL);
    printf("Thread %d with color %d is using the resource.\n",id,color);
    sleep(1);
}

int otherColorNotStarving(int color)
{
    int otherColor;
    if(color==WHITE)
    {
        //printf("Time after switch:%d ",(int)time(NULL)-lastUseBlack);
        if((int)time(NULL)-lastUseBlack<=5)
            return 1;
        else
            return 0;
    }
    else
    {
        //printf("Time after switch:%d ",(int)time(NULL)-lastUseWhite);
        if((int)time(NULL)-lastUseWhite<=5)
            return 1;
        else
            return 0;
    }
}

void stopUsingWait(int id,int color)
{
    if(colorUsing[id]==1)
    {
        colorUsing[id]=0;
        if(color==WHITE)
        {
            pthread_mutex_lock(&counter);
            waitingWhite++;
            usingWhite--;
            pthread_mutex_unlock(&counter);
            printf("Thread %d is now giving his place.\n",id);
            if(usingWhite==0)
            {
                //pthread_mutex_lock(&ownerMutex);
                printf("All white are now done, giving control to black.\n");
                owner=BLACK;
                sleep(1);
                pthread_cond_broadcast(&blackWait);
                //pthread_mutex_unlock(&ownerMutex); 
            }
        }
        else
        {
            pthread_mutex_lock(&counter);
            waitingBlack++;
            usingBlack--;
            pthread_mutex_unlock(&counter);
            printf("Thread %d is now giving his place.\n",id);
            if(usingBlack==0)
            {
                //pthread_mutex_lock(&ownerMutex);
                printf("All black are now done, giving control to white.\n");
                owner=WHITE;
                sleep(1);
                pthread_cond_broadcast(&whiteWait);
                //pthread_mutex_unlock(&ownerMutex); 
            }
        }
    }
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
        if(isOwner(color)&&otherColorNotStarving(color))
            useResource(id,color);
        else
            if(isOwner(color))
                stopUsingWait(id,color);
            else
            {
                while(!(isOwner(color)))
                {
                    pthread_mutex_lock(&ownerMutex);
                    if(color==WHITE)
                        pthread_cond_wait(&whiteWait,&ownerMutex);
                    else
                        pthread_cond_wait(&blackWait,&ownerMutex);
                    pthread_mutex_unlock(&ownerMutex);    
                }
            }
    }
}

int main()
{
    pthread_t threads[100];
    lastUseBlack=(int)time(NULL);
    waitingBlack=5;
    waitingWhite=5;
    for(int i=0;i<10;i++)
    {
        int * id = (int*)malloc (sizeof(int));
        *id=i;
        pthread_create(&threads[i],NULL,doRoutine,(void*)id);
    }
    sleep(30);
    return 0;
}
