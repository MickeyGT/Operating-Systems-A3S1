#include<stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
volatile int flag_tr1, flag_tr2;

void *threadAnswers(void* args)
{
	flag_tr1 = 1;
	for (int j = 1; j <= 10&&flag_tr2; j++)
	{
		int answer;
		printf("Question %d:", j);
		scanf("%d", &answer);
		//printf("\n");
	}
	if(flag_tr2==1)
		printf("Test completed, next user.\n");
	flag_tr1 = 0;
	flag_tr2 = 0;
}

void *threadWait(void *args)
{
	flag_tr2 = 1;
	int duration = 20;
	for (int j = 1; j <= 10&&flag_tr1 ; j++)
	{
		sleep(2);
		printf("\n %d seconds remaining\n", duration - 2);
		duration -= 2;
	}
	if(flag_tr1==1)
		printf("Time's up.\n");
	flag_tr1 = 0;
	flag_tr2 = 0;
}

int main()
{
	for (int k = 1; k <= 3; k++)
	{
		flag_tr1 = flag_tr2 = 1;
		pthread_t threadid1, threadid2;
		pthread_create(&threadid1, NULL, threadAnswers,NULL);
		pthread_create(&threadid2, NULL, threadWait,NULL);
		pthread_join(threadid1,NULL);
		pthread_join(threadid2, NULL);
	}
	printf("Program over.\n");
	return 0;
}
