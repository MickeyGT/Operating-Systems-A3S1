#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
#define LEFT 0
#define RIGHT 1

int count_in=0,count_out=0;

int dist_count[2];

pthread_mutex_t tunnel_in = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tunnel_out = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dist[2] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};

int sem[2];

void in_tunnel(int type)
{
	pthread_mutex_lock(&tunnel_in);
	printf("Masina din %d intra in tunel\n",type);
	count_in++;

	pthread_mutex_lock(&dist[type]);
	dist_count[type]--;

	pthread_mutex_unlock(&dist[type]);

	pthread_mutex_unlock(&tunnel_in);
}

void out_tunnel(int type)
{
	pthread_mutex_lock(&tunnel_out);
	printf("Masina din %d iese din tunel\n",type);

	count_out++;

	pthread_mutex_unlock(&tunnel_out);
}

void *car(void *type)
{
	int car_type = *(int*)type;
	free(type);

	pthread_mutex_lock(&dist[car_type]);
	dist_count[car_type]++;

	pthread_mutex_unlock(&dist[car_type]);

	sleep(5);

	while(sem[car_type] == 0) {;}

	in_tunnel(car_type);

	sleep(5);

	out_tunnel(car_type);

	return NULL;
}

void *control_center(void *arg)
{
	int timer;
	while(1)
	{
		timer = 0;
		count_in = 0;
		count_out = 0;
		printf("Semaforul din stanga se face verde\n");
		sem[LEFT] = 1;
		sem[RIGHT] = 0;

		while(dist_count[RIGHT] == 0){;}

		while(!(dist_count[LEFT] <5 && dist_count[RIGHT] >= 5) && count_in < 3 && (++timer)<1000000){;}

		printf("Semaforul din stanga se face rosu\n");
		sem[LEFT] = 0;

		sleep(3);

		while(count_out <count_in){;}

		count_in = 0;
		count_out = 0;
		printf("Semaforul din dreapta se face verde\n");
		sem[RIGHT] = 1;
		timer = 0;

		while(dist_count[LEFT] == 0){;}

		while(!(dist_count[LEFT] >= 5 && dist_count[RIGHT] < 5) && count_in < 3 && (++timer)<1000000){;}

		printf("Semaforul din dreapta se face rosu\n");
		sem[RIGHT] = 0;
		timer = 0;
		sleep(3);

		while(count_out < count_in){;}
	}
}

int main()
{
	pthread_t cars[100],control;

	srand(time(NULL));

	pthread_create(&control,NULL,control_center,NULL);

	for(int i = 0; i < 100; i++)
	{
		int *x = (int*)malloc (sizeof(int));
		*x = rand()%2;
		//* x = 1;
		pthread_create(&cars[i],NULL,car,x);
	}

	for(int i = 0; i < 100; i++)
	{
		pthread_join(cars[i],NULL);
	}

	pthread_join(control,NULL);

	return 0;
}
