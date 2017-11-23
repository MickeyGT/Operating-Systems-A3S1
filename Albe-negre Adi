#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#define N 10
#define WHITE 0
#define BLACK 1
#define NONE 2

int runnable = 1;

volatile int coloredFrequencies[2] = {0, 0};

pthread_mutex_t ownerMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ownerCond   = PTHREAD_COND_INITIALIZER;
pthread_mutex_t resourceMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int color;
    int id;
} ThreadInfo;

int GetOwner()
{
    if(coloredFrequencies[WHITE] != 0)
        return WHITE;

    if(coloredFrequencies[BLACK] != 0)
        return BLACK;

    return NONE;
}

int CanUse(int color)
{
    return GetOwner() == NONE || GetOwner() == color;
}

void GetResourceOwnership(int color, int id)
{
    pthread_mutex_lock(&ownerMutex);

    printf("Thread %d colored %d wants to get ownership\n", id, color);

    while(CanUse(color) == 0)
    {
        pthread_cond_wait(&ownerCond, &ownerMutex);
    }
    printf("Thread %d colored %d owns the resource\n", id, color);
    printf("Refcount: %d\n", ++coloredFrequencies[color]);

    pthread_mutex_unlock(&ownerMutex);
}

void ReleaseResourceOwnership(int color, int id)
{
    pthread_mutex_lock(&ownerMutex);

    int owner = GetOwner();

    printf("Thread %d colored %d releases the ownership over the resource\n", id                                                                                                                                                             , color);

    printf("Refcount: %d\n", coloredFrequencies[owner]--);
    if(coloredFrequencies[owner] == 0)
    {
        printf("There are no more threads having the color %d\n", color);
        pthread_cond_broadcast(&ownerCond);
    }

    pthread_mutex_unlock(&ownerMutex);
}

void UseResource(int color, int id)
{
    GetResourceOwnership(color, id);

    pthread_mutex_lock(&resourceMutex);
    printf("Thread %d colored %d is using the resource\n", id, color);
    sleep(rand() % 5 + 1);
    pthread_mutex_unlock(&resourceMutex);

    ReleaseResourceOwnership(color, id);
}

void * DoColoredThreadRoutine(void * arg)
{
    ThreadInfo info = *((ThreadInfo*)arg);
    int id = info.id;
    int color = info.color;
    free(arg);

    while(runnable)
    {
        UseResource(color, id);

        // do other stuff
        sleep(rand() % 5 + 1);
    }
}

int main()
{
    srand(time(NULL));
    pthread_t threads[N];

    for(int i = 0; i < N; i++)
    {
        ThreadInfo * info = (ThreadInfo*)malloc(sizeof(int));
        info->color = rand() % 2;
        info->id = i;
        pthread_create(&threads[i], NULL, DoColoredThreadRoutine, (void*)info);
    }

    sleep(30);

    for(int i = 0; i < N; i++)
    {
        pthread_cancel(threads[i]);
    }

    return 0;
