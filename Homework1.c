#include<stdio.h>
#include  <sys/types.h>
#include <stdlib.h>

int main()
{
	for (int k = 1; k <= 10; k++)
	{
		pid_t pid=fork();
		if(pid==0)
		{
		    //this is child
		    int duration=60;
		    for(int i=1; i<=12;i++)
		        {
		            sleep(5);
		            printf("\n %d seconds remaining\n",duration-5);
		            duration-=5;
		        }
		    printf("Time's up.\n");
		    exit(1);
		}
		else
		{
		    //this is mainparent
		    pid_t spid=fork();
		    if(spid==0)
		    {
		        for(int j=1;j<=10;j++)
		        {
		            int answer;
		            printf("Question %d:",j);
		            scanf("%d",&answer);
		            printf("\n");
		        }
		        printf("Test completed, next user.\n");
		        exit(2);
		        //this is second child
		    }
		    else
		    {
		        wait();
		        kill(pid,9);
		        kill(spid,9);
		        //this is mainparent
		    }
		}
	}
	printf("Program over.\n");
	return 0;
}
