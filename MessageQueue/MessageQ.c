
#include <fcntl.h>
#include <mqueue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"


void * queue_server(void * args) {
	mqd_t mq;
	ssize_t bytes_read;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];

    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    /* create the message queue */
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attr);

    while(1) {
		
		memset(buffer, 0x00, sizeof(buffer));
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
        if(bytes_read >= 0) {
			printf("SERVER: Received message: %s\n", buffer);
		} else {
			printf("SERVER: None \n");
		}
		
		fflush(stdout);
		usleep(0.725 * 1e6);
    }

    /* cleanup */
    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return NULL;
}


void * queue_client(void * args) {
	
	mqd_t mq;
    char buffer[MAX_SIZE];

    /* open the mail queue */
    mq = mq_open(QUEUE_NAME, O_WRONLY);

	int count = 0;
    while(1) {
        snprintf(buffer, sizeof(buffer), "MESSAGE %d", count++);
		
		printf("CLIENT: Send message... \n");
        mq_send(mq, buffer, MAX_SIZE, 0);
		
		fflush(stdout);
		usleep(7.33 * 1e6);
    }

    mq_close(mq);

    return NULL;
}


int main(int argc, char** argv) {

	pthread_t client, server;
	
	printf("Start...\n");
    
	pthread_create(&server, NULL, &queue_server, NULL);
	pthread_create(&client, NULL, &queue_client, NULL);
	
	pthread_join(server, NULL);
	pthread_join(client, NULL);
	
	printf("Done...\n");
	
	return (EXIT_SUCCESS);
}