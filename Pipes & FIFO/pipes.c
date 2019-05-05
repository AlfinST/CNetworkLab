#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

struct ListOfNames
{
    char names[50][10];
    int n;
}L,L1;

int main()
{
    int i,j,fd[2];
    pid_t p;
    char temp[10];

    if(pipe(fd)==-1)
    {
        fprintf(stderr,"pipe failed"); //not really needed
        return 1;
    }
    //enter names
    printf("Enter number of names:");
    scanf("%d",&L.n);
    for(i=0;i<L.n;i++)
        scanf("%s",&L.names[i]);
    p=fork();
    if(p<0)
    {
        fprintf(stderr,"pipe failed"); //not really needed
        return 1;
    }
    else if(p>0)
    {
        close(fd[0]);
        printf("\n\nSending:\n");

        for(i=0;i<L.n;i++)
        {
            printf("%s\n",L.names[i]);
        }
        write(fd[1],&L,sizeof(L));
        close(fd[1]);
    }
    else
    {
        close(fd[1]);
        read(fd[0],&L1,sizeof(L1));
        printf("\n\npipes are read\n");

        for(i = 0; i < L1.n - 1 ; i++)
        {
            for (j = i + 1; j < L1.n; j++)
            {
                if (strcmp(L1.names[i], L1.names[j]) > 0) 
                {
                    strcpy(temp, L1.names[i]);
                    strcpy(L1.names[i], L1.names[j]);
                    strcpy(L1.names[j], temp);
                }
            }
        }

        for(i=0;i<L1.n;i++)
        {
            printf("%s\n",L1.names[i]);
        }
        close(fd[0]);
        exit(0);

    }
    
}