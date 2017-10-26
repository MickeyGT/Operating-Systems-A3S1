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
    int v[12]={0,2,3,4,0,0,0,5,6,0,1,7};
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
                    exit(5);
            }
            printf("My thread didn't find any 1 %d \n",i);
            exit(4);
        }
    }
    int status;
    int ok=0;
    for(i = 1; i <= nrProces; i++)
    {
        wait(& status);
        if(WIFEXITED(status))
                if(WEXITSTATUS(status)==5)
                        ok=1;
    }
    if(ok)
        printf("Am gasit 1\n");
    else
        printf("Nu am gasit 1\n");
    return 0;
}
