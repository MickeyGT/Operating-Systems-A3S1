#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
#define LEFT 0
#define RIGHT 1

pthread_mutex_t counter = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t waitLeft = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t waitRight = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t tunnelWaitLeft = PTHREAD_COND_INITIALIZER;
pthread_cond_t tunnelWaitRight = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ownerMutex = PTHREAD_MUTEX_INITIALIZER;

int semafor[2]={0,0};
int carsin,carsWaitLeft,carsWaitRight;
int lastUseRight,lastUseLeft;

void* car(void *arg)
{
    int direction = *(int*)arg;
    if(direction==LEFT)
    {
        pthread_mutex_lock(&waitLeft);
        carsWaitLeft++;
        pthread_mutex_unlock(&waitLeft);
    }
    else
    {
        pthread_mutex_lock(&waitRight);
        carsWaitRight++;
        pthread_mutex_unlock(&waitRight);
    }
	free(arg);
	//sleep(5);

    if(direction==LEFT)
    {
        pthread_mutex_lock(&ownerMutex);
        pthread_cond_wait(&tunnelWaitLeft,&ownerMutex);
        printf("Car coming from direction LEFT entering tunnel.\n");
	    pthread_mutex_unlock(&ownerMutex);
	    sleep(5);
	    lastUseLeft=(int)time(NULL);
	    pthread_mutex_lock(&counter);
	    carsin--;
	    pthread_mutex_unlock(&counter);
	    printf("Car coming from direction LEFT exiting tunnel.\n");
	    return;
    }
    else
    {
        pthread_mutex_lock(&ownerMutex);
        pthread_cond_wait(&tunnelWaitRight,&ownerMutex);
        printf("Car coming from direction RIGHT entering tunnel.\n");
        pthread_mutex_unlock(&ownerMutex);
        sleep(5);
        lastUseRight=(int)time(NULL);
        pthread_mutex_lock(&counter);
	    carsin--;
	    pthread_mutex_unlock(&counter);
        printf("Car coming from direction RIGHT exiting tunnel.\n");
        return;
    }
}

int otherSideNotStarving(int type)
{
    int currTime;
    currTime=(int)time(NULL);
    if(type==LEFT)
    {
        if(carsWaitRight==0)
            return 1;
        else
        {
            //printf("Time after switch:%d ",(int)time(NULL)-lastUseRight);
            if(currTime-lastUseRight<=4)
                return 1;
            else
                return 0;
        }
    }
    else
    {
        if(carsWaitLeft==0)
            return 1;
        else
        {
            //printf("Time after switch:%d ",(int)time(NULL)-lastUseLeft);
            if(currTime-lastUseLeft<=4)
                return 1;
            else
                return 0;
        }
    }
}

void * controlCenter(void *arg)
{
    sleep(3);
    semafor[LEFT]=1;
    lastUseRight=(int)time(NULL);
    printf("LEFT side semaphore is GREEN\n");
    printf("RIGHT side smeaphore is RED\n");
    while(1)
    {
        if(semafor[LEFT]==1)
        {
            while(otherSideNotStarving(LEFT)&&carsWaitLeft!=0)
                {
                    printf("Clearing 1 car coming from LEFT to enter tunnel.\n");
                    pthread_mutex_lock(&counter);
	                carsin++;
	                pthread_mutex_unlock(&counter);
	                pthread_mutex_lock(&waitLeft);
                    carsWaitLeft--;
                    pthread_mutex_unlock(&waitLeft);
	                pthread_cond_signal(&tunnelWaitLeft);
	                sleep(1);
                }
            while(carsin!=0){;}
            if(carsWaitRight!=0)
            {
                semafor[LEFT]=0;
                printf("LEFT side semaphore is RED\n");
                semafor[RIGHT]=1;
                printf("RIGHT side smeaphore is GREEN\n");
            }
            else
                if(carsWaitLeft==0)
                {
                    printf("No more cars on either side. Exiting.\n");
                }
                else
                {
                    printf("No cars on right side, keeping left semaphore GREEN\n");
                }
        }
        if(semafor[RIGHT]==1)
        {
            while(otherSideNotStarving(RIGHT)&&carsWaitRight!=0)
                {
                    printf("Clearing 1 car coming from RIGHT to enter tunnel.\n");
                    pthread_mutex_lock(&counter);
	                carsin++;
	                pthread_mutex_unlock(&counter);
	                pthread_mutex_lock(&waitRight);
                    carsWaitRight--;
                    pthread_mutex_unlock(&waitRight);
	                pthread_cond_signal(&tunnelWaitRight);
	                sleep(1);
                }
            while(carsin!=0){;}
            if(carsWaitLeft!=0)
            {
                semafor[RIGHT]=0;
                semafor[LEFT]=1;
                printf("LEFT side semaphore is GREEN\n");
                printf("RIGHT side smeaphore is RED\n");
            }
            else
                if(carsWaitLeft==0)
                {
                    printf("No more cars on either side. Exiting.\n");
                }
                else
                {
                    printf("No cars on left side, keeping right semaphore GREEN");
                }
        }
    }
}

int main()
{
    srand(time(NULL));
    pthread_t threads[100];
    pthread_t control_center;
    pthread_create(&control_center,NULL,controlCenter,NULL);
    for(int i=0;i<30;i++)
    {
        int * direction = (int*)malloc (sizeof(int));
        //*direction = rand()%2;
        if(i<=3)
            *direction=0;
        else
            *direction=1;
        pthread_create(&threads[i],NULL,car,(void*)direction);
    }
    sleep(300);
    return 0;
}














