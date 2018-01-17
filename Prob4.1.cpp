#include<stdlib.h>
#include<stdio.h>
#include <windows.h>
#include<sys/types.h>
#define WRITE 1
#define READ 0
int i;
int v[12] = { 0,2,3,4,0,0,0,5,6,0,1,7 };
int nrProces = 5;
DWORD WINAPI ThreadFunc(LPVOID data) 
{
	int nrThread = *((int*)data);
	int j;
	free(data);
	for (j = nrThread; j <= 11; j+=nrProces)
	{
		if (v[j] == 1)
		{
			printf("My thread found an 1. %d \n", nrThread);
			return 1;
		}
	}
	printf("My thread didn't find any 1 %d \n", nrThread);
	return 0;
}

int main()
{
	int ok = 0;
	HANDLE threads[5];
	for (i = 0; i < nrProces; i++)
	{
		int* nr = (int*)malloc(sizeof(int));
		*nr = i;
		threads[i]=CreateThread(NULL,0,ThreadFunc,nr,0,NULL);
	}
	WaitForMultipleObjects(5, threads, true, INFINITE);
	for (i = 0; i < nrProces; i++)
	{
		DWORD exitcode;
		int status = GetExitCodeThread(threads[i],&exitcode);
		if (exitcode == 1)
			ok = 1;
	}
	if (ok)
		printf("Am gasit 1\n");
	else
		printf("Nu am gasit 1\n");
	return 0;
}
