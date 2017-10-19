    I A  program.c (c)                                                       Row 2    Col 1    4:56  Ctrl-K H for help
#include<stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
        for (int k = 1; k <= 10; k++)
        {
                pid_t pid=fork();
                if(pid==0)
                {
                    //this is child
                    int duration=20;
                    for(int i=1; i<=10;i++)
                        {
                            sleep(2);
                            printf("\n %d seconds remaining\n",duration-2);
                            duration-=2;
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
                        int *status;
                        pid_t finished = wait(status);
                        if(finished==pid)
                            kill(spid,9);
                        else
                            kill(pid,9);
                        wait(NULL);
                        //this is mainparent
                    }
                }
        }
        printf("Program over.\n");
        return 0;
}
