#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
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
sem_t *buf_mutex,*empty_count,*fill_count,*spool_signal_sem ;
struct shared_memory *shared_memory_ptr;


void producer(int Active_Producer,int x){

	while(x--){
		int i,p;
		sem_wait(empty_count);
		sem_wait(buf_mutex);
         //critical scetion
            p = 1 + rand()%40;
		    ++(shared_memory_ptr->buffer_pos);

            shared_memory_ptr->Action=1;
            shared_memory_ptr->by=Active_Producer;

            *((shared_memory_ptr->Buffer)+(shared_memory_ptr->buffer_pos))=p;  //*(buf + buf_pos) = p; 
            printf("Producer %d produced %d  \n",Active_Producer,p);
		sem_post(buf_mutex);
		sem_post(fill_count);
        sem_post(spool_signal_sem);
		sleep(1 + rand()%3);
	}

}

int main(int argc , char ** argv)
{
    int Active_Producer,fd_shm;
    srand(time(NULL));
    /////////////////////////////////////////////////////////////////////////////////////
    if ((buf_mutex = sem_open (SEM_MUTEX_NAME, O_EXCL, 0660, 1)) == SEM_FAILED)
        error ("sem_mutex_open");
    
    if ((fill_count = sem_open (SEM_FILL_NAME, O_EXCL, 0660, 0)) == SEM_FAILED)
        error ("sem_fill_open");
    
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

    if ((empty_count = sem_open (SEM_EMTY_NAME, O_EXCL, 0660, shared_memory_ptr->buffer_cap)) == SEM_FAILED)
        error ("sem_count_open");
    printf("Initialisation Done\n");

    /////////////////////////////////////////////////////////////////////////////////////

    Active_Producer = (shared_memory_ptr->Number_of_Producers)+1;
    (shared_memory_ptr->Number_of_Producers)++;
    printf("New Producer! No: %d\n",Active_Producer);
    producer(Active_Producer,10);

}

void error (char *msg)
{
    perror (msg);
    exit (1);
}