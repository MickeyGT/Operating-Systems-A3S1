#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int i,nrProces;
int v[12] = { 1,2,3,4,1,1,1,5,6,1,1,7 };
void *thread(void *arg)
{
	int nrCurrentThread = *((int*)arg);
	free(arg);
	printf("Am pornit thread-ul %d \n", nrCurrentThread);
	int* sum = (int*) malloc(sizeof(int));
	int j;
	*sum = 0;
	for (j = nrCurrentThread* 3; j <= 11 && j <= nrCurrentThread * 3 + 2; j++)
	{
		*sum += v[j];
	}
	return (void*)sum;
}

int main()
{
	pthread_t thread_id[5];
	int nrProces = 5;
	for (i = 0; i < nrProces; i++)
	{
		//Child process.
		int* nr = (int*) malloc(sizeof(int));
		*nr = i;
		pthread_create(&thread_id[i], NULL, thread, (void*)nr);
	}
	int totalsum = 0;
	for(i=0;i<nrProces;i++)
	{
		int *rez;
		pthread_join(thread_id[i], (void**)&rez);
		totalsum += *rez;
		free(rez);
	}
	printf("Suma este: %d  \n", totalsum);
	return 0;
}
