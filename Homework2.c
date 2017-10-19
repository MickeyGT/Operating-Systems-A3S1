#include <stdio.h>
#include <unistd.h> 
int v[1001];
int main()
{
    int pipe(int filedes[2]); 
    for(int i=1;i<=100;i++)
        v[i]=i%3;
    int n;
    scanf("%d",&n);
    
    return 0;
}
