#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
#define BLACK 1
#define WHITE 0
#define INTERMEDIATE 2
int owner=WHITE,usingWhite=0,usingBlack=0,waitingWhite=0,waitingBlack=0;
int lastUseBlack,lastUseWhite;
pthread_mutex_t counter = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t whiteWait = PTHREAD_COND_INITIALIZER;
pthread_cond_t blackWait = PTHREAD_COND_INITIALIZER;
pthread_cond_t counterWait = PTHREAD_COND_INITIALIZER;
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
    {
        usingWhite--;
        if(usingWhite==0)
            pthread_cond_signal(&counterWait);
    }
    else
    {
        usingBlack--;
        if(usingBlack==0)
            pthread_cond_signal(&counterWait);
    }
    pthread_mutex_unlock(&counter);
}

int colorStarving(int color)
{
    if(color==WHITE)
    {
        printf(" %d",(int)time(NULL)-lastUseWhite);
        if((int)time(NULL)-lastUseWhite<=5)
            return 0;
        else
        {
            if(waitingWhite==0)
            {
                printf("Color WHITE is starving but there are no WHITE threads waiting.\n");
                return 0;
            }
            return 1;
        }
    }
    else
    {
        printf(" %d",(int)time(NULL)-lastUseBlack);
        if((int)time(NULL)-lastUseBlack<=5)
            return 0;
        else
        {
            if(waitingBlack==0)
            {
                printf("Color BLACK is starving but there are no BLACK threads waiting.\n");
                return 0;
            }
            return 1;
        }
    }
}

void switchOwner(int id,int color)
{
    if(!isOwner(INTERMEDIATE))
    {
        printf("Thread %d with color %d is switching the owner color.\n",id,color);
        if(color==WHITE)
        {
            owner=INTERMEDIATE;
            while(usingWhite!=0)
            {
                pthread_cond_wait(&counterWait,&counter);
            }
            printf("All white are now done, giving control to black.\n");
            sleep(1);
            lastUseWhite=(int)time(NULL);
            pthread_cond_broadcast(&blackWait);
            owner=BLACK;
        }
        else
        {
            owner=INTERMEDIATE;
            while(usingBlack!=0)
            {
                pthread_cond_wait(&counterWait,&counter);
            }
            printf("All black are now done, giving control to white.\n");
            sleep(1);
            lastUseBlack=(int)time(NULL);
            pthread_cond_broadcast(&whiteWait);
            owner=WHITE;
        }
    }
}

int isOwner(int myColor)
{
    return myColor==owner;
}

void getAccess(int id,int color)
{
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
    
}

struct proces
{
    int id;
    int color;
}temp;

typedef struct proces proces;

void releaseAccess(int id,int color)
{
    if((color==WHITE&&colorStarving(BLACK))||(color==BLACK&&colorStarving(WHITE)))
        switchOwner(id,color);
}

void* doRoutine(void *arg)
{
    proces pr;
    pr = *((proces*)arg);
    free(arg);

    getAccess(pr.id,pr.color);
    
    useResource(pr.id,pr.color);
    
    releaseAccess(pr.id,pr.color);
    
    return;
}

int main()
{
    pthread_t threads[300];
    lastUseBlack=(int)time(NULL);
    lastUseWhite=(int)time(NULL);
    for(int i=0;i<50;i++)
    {
        //color= WHITE;
        srand(time(NULL));
        int clr = rand()%2;
        proces * tr = (proces*)malloc (sizeof(proces));
        tr->id=i;
        tr->color=clr;
        pthread_create(&threads[i],NULL,doRoutine,(void*)tr);
        sleep(1);
    }
    sleep(205);
    return 0;
}







