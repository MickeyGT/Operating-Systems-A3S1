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
            int j;
            for(j = i*3; j <= 11&&j<=i*3+2; j++)
            {
                if(v[j] == 1)
                    write(myPipe[WRITE], &j, sizeof(j));
            }
            exit(0);
        }
    }
    close(myPipe[WRITE]);
    int poz;
    printf("Am gasit pe 1 la pozitiile:\n ");
    while(read(myPipe[READ], &poz, sizeof(poz)))
    {
        printf("%d, ", poz);
    }
    printf("\n");
    for(i = 1; i <= nrProces; i++)
    {
        wait(0);
    }
    return 0;
}
