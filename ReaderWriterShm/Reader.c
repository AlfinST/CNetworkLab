#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>


#define SEM_RMUTEX_NAME "/sem-Rmutex"
#define SEM_WMUTEX_NAME "/sem-Wmutex"
#define SEM_READTRY_NAME "/sem-Readtry"
#define SEM_SPOOL_SIGNAL_NAME "/sem-spool-signal"
#define SEM_RESOURCE_NAME "/sme-resource"
#define SHARED_MEM_NAME "/posix-shared-mem-example"
#define MAX_BUFFER_SIZE 100

void error(char *msg);

struct shared_memory{
    int Buffer[MAX_BUFFER_SIZE];
    int ReadCount;
    int Number_of_Readers;
    int Number_of_Writers;
    int WriteCount;
    int buffer_pos;
    int Action,by;
};

int main(void){
	
    struct shared_memory *shared_memory_ptr;
	int fd_shm;
    sem_t *rmutex, *wmutex, *readTry, *resource,*spool_signal_sem;//*buf_mutex,*empty_count,*fill_count,*spool_signal_sem,*sem_print ;

    int Active_Reader;
    int buf_pos=0,i=0;

        // printf("Enter buffer capacity(upto 100):");
        // scanf("%d",&buf_len);

    if ((rmutex = sem_open (SEM_RMUTEX_NAME, O_EXCL, 0660, 1)) == SEM_FAILED)
        error ("sem_Rmutex_open");
    
    // if ((wmutex = sem_open (SEM_WMUTEX_NAME, O_EXCL, 0660, 1)) == SEM_FAILED)
    //     error ("sem_Wmutex_open");

    if ((readTry = sem_open (SEM_READTRY_NAME, O_EXCL, 0660,1)) == SEM_FAILED)
        error ("sem_RTry_open");
    
    if ((resource = sem_open (SEM_RESOURCE_NAME, O_EXCL, 0660, 1)) == SEM_FAILED)
        error ("sem_Resource_open");

    if ((spool_signal_sem = sem_open (SEM_SPOOL_SIGNAL_NAME, O_EXCL, 0660, 0)) == SEM_FAILED)
        error ("sem_spool_open");
    printf("All Semaphors Setup!\n");

     if ((fd_shm = shm_open (SHARED_MEM_NAME, O_EXCL|O_RDWR, S_IRWXU)) == -1)
        error ("shm_open");
    printf("Shared Memory Setup!\n");

    if (ftruncate (fd_shm, sizeof (struct shared_memory)) == -1)
       error ("ftruncate");
    printf("Memory Truncated! \n");

     if (( shared_memory_ptr = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm, 0)) == MAP_FAILED)
       error ("mmap");
    printf("Memory Mapped\n");

    Active_Reader=(shared_memory_ptr->Number_of_Readers)+1;
    (shared_memory_ptr->Number_of_Readers)++;


    printf("Initiation done!\n");

    //ENTRY SECTION
    while(1){
        //printf("" ReadTry");
        if(sem_wait(readTry)==-1)
            error("sem_wait:ReadTry");
        printf("Sem ReadTry got Mut\n");
        if(sem_wait(rmutex)==-1)
            error("sem_wait:RMutex");
        printf("Sem RMtex done");

        (shared_memory_ptr->ReadCount)++;
        if (shared_memory_ptr->ReadCount ==1)
            if(sem_wait(resource)==-1)
                error("sem_wait:Resource");
            printf(" SemResource Held\n");
        
        if(sem_post(rmutex)==-1)
            error("sem_post:Rmutex");
            printf("Release Rmutex");
        
        if(sem_post(readTry)==-1)
            error("sem_post:ReadTry");
        printf("Release ReadTry");
        //CRITICAL SECTION

        //printf("In Critical Section of Reader %d",Active_Reader);
        shared_memory_ptr->Action=1;
        shared_memory_ptr->by=Active_Reader;
        printf("Buffer is: ");
        for(i=0;i<(shared_memory_ptr->buffer_pos);i++)
            printf("%d ",*(shared_memory_ptr->Buffer+i));
        printf("\n");

        //EXIT SECTION
        if(sem_wait(rmutex)==-1)
            error("sem_wait:RMutex");
        printf("Sem rmutex Helf");
        (shared_memory_ptr->ReadCount)--;
        if (shared_memory_ptr->ReadCount ==0)
            if(sem_post(resource)==-1)
                error("Sem_post:Resource");
        if(sem_post(rmutex)==-1)
            error("sem_post:Rmutex");
        sem_post(spool_signal_sem);
		sleep(1 + rand()%3);
         
    }
}
void error (char *msg)
{
    perror (msg);
    exit (1);
}


