#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

#define SEM_MUTEX_NAME "/sem-mutex"
#define SHARED_MEM_NAME "/posix-shared-mem-example"
#define SEM_SPOOL_SIGNAL_NAME "/sem-spool-signal"

struct shared_memory
{
    int Number;
    int Number_of_Clients;
    int Active_Client;
};

void error (char *msg);

int main (int argc, char **argv)
{
    struct shared_memory *shared_memory_ptr;
    sem_t *mutex_sem, *spool_signal_sem ;
    int fd_shm;


    if ((mutex_sem = sem_open (SEM_MUTEX_NAME ,O_CREAT, 0644, 0)) == SEM_FAILED)
        error ("sem_open");
    printf("Mutex Sem Done !\n");


    if ((fd_shm = shm_open (SHARED_MEM_NAME, O_CREAT|O_RDWR, S_IRWXU)) == -1)
        error ("shm_open");
    printf("Shared Memory Setup!\n");


    if (ftruncate (fd_shm, sizeof (struct shared_memory)) == -1)
       error ("ftruncate");
    printf("Memory Truncated! \n");


     if (( shared_memory_ptr = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm, 0)) == MAP_FAILED)
       error ("mmap");
    printf("Memory Mapped\n");


    shared_memory_ptr ->Number=0;
    shared_memory_ptr ->Number_of_Clients=0;
    
    if ((spool_signal_sem = sem_open (SEM_SPOOL_SIGNAL_NAME, O_CREAT, 0644, 0)) == SEM_FAILED)
         error ("sem_open");
    printf("Spool Semaphor Done!\n");

    //Initialisation over
    printf("Initialisation Over!\n");

    if (sem_post (mutex_sem) == -1)
        error ("sem_post: mutex_sem");

    while(1)
    {
          // forever
       if (sem_wait (spool_signal_sem) == -1)
            error ("sem_wait: spool_signal_sem");

        
        
        printf("Value of Number is: %d \t Incremented By %d\n",shared_memory_ptr->Number,shared_memory_ptr->Active_Client);
           
        if(shared_memory_ptr->Number ==(10*(shared_memory_ptr->Number_of_Clients)))
        {
            printf("\nEnough With the incremnting\nServer Shuting Down!\n");   
            exit(1);
        }
        
        
    }
}

// Print system error and exit
void error (char *msg)
{
    perror (msg);
    exit (1);
}
