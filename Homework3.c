
    I    homework2.c (Modified)                                           Row 43   Col 2    4:10  Ctrl-K H for help
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define WRITE 1
#define READ 0
int i;
pid_t pid;
int main()
{
    int v[12]={1,2,3,4,1,1,1,5,6,1,1,7};
    int nrProces = 5;
    for(i = 0; i < nrProces; i++)
    {
        pid=fork();
        if(pid == 0)
        {
            //Child process.
            int j;
            for(j = i*3; j <= 11&&j<=i*3+2; j++)
            {
                if(v[j] == 1)
                    exit(1);
            }
            exit(0);
        }
    }
    int *status;
    int ok=0;
    printf("\n");
    for(i = 1; i <= nrProces; i++)
    {
        wait(status);
        if(WEXITSTATUS(status)==1)
                ok=1;
    }
    if(ok)
        printf("Am gasit 1");
    else
        printf("Nu am gasit 1");
    return 0;
}

