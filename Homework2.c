    I    array.c                                 Row 1    Col 18   5:35  Ctrl-K H for help
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define WRITE 1
#define READ 0

int main()
{
        int arr[10]={2,3,4,2,5,2,1,4,3,2};
        int searchN  = 2;
        int NrProces = 5;
        int pipefd[2];
        int i;
        pipe(pipefd);
        pid_t ID;
        for(i = 1; i <= NrProces; i++)
        {
                ID=fork();
                if(ID == 0)
                {
                        int j;
                        for(j = (i-1)*(10/NrProces); j <= i*(10 / NrProces)-1; j++)
                        {
                                if(arr[j] == searchN)
                                        write(pipefd[WRITE], &j, sizeof(j));
                        }
                        exit(0);
                }
        }
        close(pipefd[WRITE]);
        int poz;
        printf("Numarul %d a fost gasit la poziti▒ile:\n ", searchN);
        while(read(pipefd[READ], &poz, sizeof(poz)))
        {
                printf("%d, ", poz);
        }
        printf("\n");
        for(i = 1; i <= NrProces; i++)
        {
                wait(0);
        }
        return 0;
}
