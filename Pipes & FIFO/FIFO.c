#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include <sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>

struct ListOfNames
{
    char names[50][10];
    int n;
}L,L1;
int main()
{
    int fd,i,j;
    char temp[10];
    pid_t p;
    if(mkfifo("my_fifo",0666)<0)
        perror("mkfifo");
    printf("Enter number of names :");
    scanf("%d",&L.n);
    printf("Enter Names: \n");
    for(i=0;i<L.n;i++)
        scanf("%s",&L.names[i]);

    p=fork();
    if(p<0)
    {
        fprintf(stderr,"Fork Failed");
        return 1;
    }
    else if(p>0)
    {
        fd=open("my_fifo",O_WRONLY);
        printf("\n\nSending:\n");

        for(i=0;i<L.n;i++)
        {
            printf("%s\n",L.names[i]);
        }
        write(fd,&L,sizeof(L));
        close(fd);
        wait(0);
    }
    else
    {
        wait(0);
        fd=open("my_fifo",O_RDONLY);
        read(fd,&L1,sizeof(L1));
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
        printf("\n\nNames are :\n");
        for(i=0;i<L1.n;i++)
        {
            printf("%s\n",L1.names[i]);
        }
        unlink("my_fifo");
        exit(0);
    }
    
}