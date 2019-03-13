#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

#define SEM_MUTEX_NAME "/sem-mutex"
#define SEM_BUFFER_COUNT_NAME "/sem-buffer-count"
#define SEM_SPOOL_SIGNAL_NAME "/sem-spool-signal"
#define SHARED_MEM_NAME "/posix-shared-mem-example"

struct shared_memory
{
    int Number;
    int Number_of_Clients;
    int Active_Client;
};

void error (char *msg);

int main (int argc, char **argv)
{
    int x=10,client_number;
    struct shared_memory *shared_mem_ptr;
    sem_t *mutex_sem, *spool_signal_sem;
    int fd_shm;

        // Get shared memory 
    if ((fd_shm = shm_open (SHARED_MEM_NAME, O_EXCL|O_RDWR, S_IRWXU)) == -1)
        error ("shm_open");
    printf("Shared Memory accessed\n");

    if ((mutex_sem = sem_open (SEM_MUTEX_NAME,O_EXCL, 0660,0)) == SEM_FAILED)
        error ("sem_open");
    printf("Mutex Sem Done !\n");

    if ((shared_mem_ptr = mmap (NULL, sizeof (struct shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED,fd_shm, 0)) == MAP_FAILED)
       error ("mmap");
    printf("Memory Mapped\n");
    

    if ((spool_signal_sem = sem_open (SEM_SPOOL_SIGNAL_NAME,O_EXCL, 0660, 0)) == SEM_FAILED)
        error ("sem_open");
    printf("Spool Done\n");

    client_number=(shared_mem_ptr->Number_of_Clients)+1;
    (shared_mem_ptr->Number_of_Clients)++;

    while (x--) {
        
        /* There might be multiple producers. We must ensure that 
            only one producer uses buffer_index at a time.  */
        // P (mutex_sem);
        if (sem_wait (mutex_sem) == -1)
            error ("sem_wait: mutex_sem");

	    // Critical section
                                   
            (shared_mem_ptr->Number)++;
            (shared_mem_ptr->Active_Client)=client_number;
    // Release mutex sem: V (mutex_sem)
        if (sem_post (mutex_sem) == -1)
            error ("sem_post: mutex_sem");
    
	// Tell spooler that there is a string to print: V (spool_signal_sem);
        if (sem_post (spool_signal_sem) == -1)
            error ("sem_post: (spool_signal_sem");

    printf ("\t Process %d incremented The Number\n",shared_mem_ptr->Active_Client);
        sleep(1);
    }
}

// Print system error and exit
void error (char *msg)
{
    perror (msg);
    exit (1);
}