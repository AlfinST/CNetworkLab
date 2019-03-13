#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

#define SEM_MUTEX_NAME "/sem-mutex"
#define SHARED_MEM_NAME "/posix-shared-mem-example"
#define SEM_SPOOL_SIGNAL_NAME "/sem-spool-signal"
struct shared_memory
{
    int Number;

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
    
        //strcpy (mybuf, shared_mem_ptr -> buf [shared_mem_ptr -> buffer_print_index]);
        
        if (shared_memory_ptr->Number ==19){
            printf("\nEnough With the incremnting\nServer Shuting Down!");   
            exit(1);}
        else
        {
            printf("Value of Number is: %d\n",shared_memory_ptr->Number);
        }
        
    }
}

// Print system error and exit
void error (char *msg)
{
    perror (msg);
    exit (1);
}