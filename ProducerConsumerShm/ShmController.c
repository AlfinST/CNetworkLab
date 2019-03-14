#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>


#define SEM_MUTEX_NAME "/sem-mutex"
#define SEM_FILL_NAME "/sem-fill"
#define SEM_EMTY_NAME "/sem-empty"
#define SEM_SPOOL_SIGNAL_NAME "/sem-spool-signal"
#define SHARED_MEM_NAME "/posix-shared-mem-example"
#define MAX_BUFFER_SIZE 100

void error(char *msg);

struct shared_memory{
    int Buffer[MAX_BUFFER_SIZE];
    int Number_of_Producers;
    int Number_of_Consumers;
    int buffer_pos;
    int buffer_cap;
    int Action,by;
};

int main(void){
	
    struct shared_memory *shared_memory_ptr;
	int fd_shm;
    sem_t *buf_mutex,*empty_count,*fill_count,*spool_signal_sem,*sem_print ;

    int buf_len,i;

	printf("Enter buffer capacity(upto 100):");
	scanf("%d",&buf_len);

    if ((buf_mutex = sem_open (SEM_MUTEX_NAME, O_CREAT, 0660, 1)) == SEM_FAILED)
        error ("sem_mutex_open");
    
    if ((fill_count = sem_open (SEM_FILL_NAME, O_CREAT, 0660, 0)) == SEM_FAILED)
        error ("sem_fill_open");

    if ((empty_count = sem_open (SEM_EMTY_NAME, O_CREAT, 0660, buf_len)) == SEM_FAILED)
        error ("sem_count_open");
    
    if ((sem_print = sem_open ("/sem-print", O_CREAT, 0660, 1)) == SEM_FAILED)
        error ("sem_count_open");

    if ((spool_signal_sem = sem_open (SEM_SPOOL_SIGNAL_NAME, O_CREAT, 0660, 0)) == SEM_FAILED)
        error ("sem_spool_open");
    printf("All Semaphors Setup!\n");

     if ((fd_shm = shm_open (SHARED_MEM_NAME, O_CREAT|O_RDWR, S_IRWXU)) == -1)
        error ("shm_open");
    printf("Shared Memory Setup!\n");


    if (ftruncate (fd_shm, sizeof (struct shared_memory)) == -1)
       error ("ftruncate");
    printf("Memory Truncated! \n");

     if (( shared_memory_ptr = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm, 0)) == MAP_FAILED)
       error ("mmap");
    printf("Memory Mapped\n");

    printf("Initialisation Done\n");

    shared_memory_ptr->buffer_cap = buf_len;
    shared_memory_ptr->Number_of_Consumers = 0;
    shared_memory_ptr->Number_of_Producers = 0;
    shared_memory_ptr->buffer_pos= -1;
    if (sem_post (buf_mutex) == -1)
        error ("sem_post: mutex_sem");

    while(1)
    {
          // forever
       if (sem_wait (spool_signal_sem) == -1)
            error ("sem_wait: spool_signal_sem");
        printf("--\n"); //For some reason i need this for the Bufer to be print (-_-)?

        //Maybe add a new PRINT_semaphor here? 
        if (sem_wait (sem_print) == -1)
            error ("sem_wait: sem_print_sem");
        printf("-!\n"); // Semaphors are weird... 
        
        if(shared_memory_ptr->Action ==1)
            printf("Producer %d Added to buffer\n",shared_memory_ptr->by);
        else
            printf("Consumer %d Consumed from buffer\n",shared_memory_ptr->by);
        printf("Buffer :");
        for(i=0;i<=shared_memory_ptr->buffer_pos;i++)
        {
            printf("%d ",shared_memory_ptr->Buffer[i]);
            
        }
        //End that new Sem here?
        sem_post(sem_print);
        
    }
}


// Print system error and exit
void error (char *msg)
{
    perror (msg);
    exit (1);
}


