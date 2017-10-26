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
    int myPipe[2];
    pipe(myPipe);
    for(i = 0; i < nrProces; i++)
    {
        pid=fork();
        if(pid == 0)
        {
            //Child process.
            int j,sum=0;
            for(j = i*3; j <= 11&&j<=i*3+2; j++)
            {
                sum+=v[j];
            }
            write(myPipe[WRITE],&sum,sizeof(sum));
            exit(0);
        }
    }
    close(myPipe[WRITE]);
    int nr,totalsum=0;
    while(read(myPipe[READ], &nr, sizeof(nr)))
    {
        totalsum+=nr;
    }
    printf("Suma este: %d  \n",totalsum );
    for(i = 1; i <= nrProces; i++)
    {
        wait(0);
    }
    return 0;
}
