# У ада 3 врата: похоть, гнев и жадность.
## To compile it use
    gcc ShmController.c -o Cntrl -lpthread -lrt
    gcc ProducerShm.c -o Cntrl -lpthread -lrt
    gcc ConsumerShm.c -o Cntrl -lpthread -lrt
## How to Run :
    1. Run the Controller with ./Cntrl . Specify the buffer size. Get Initialisation Setup Messages. By default it runs forever.
    2. Run the Producer with ./Producer .Get Initialisation Setup Messages. By default it stops after creating 10 items in buffer.
    3. Run the Consumer with ./Consumer .Get Initialisation Setup Messages. By default it stops after consuming 10 items from buffer.

## It's adviced to remove the files in /dev/shm/ as it might affect all programs after a few runs.
    > you have 3 hours figure it yourself on how to make it more clean and aviod this.
## How to Control the program
    -you can change the frequency of activity by changing the sleep() in producer and consumer function in corresponding programs.

    -The last function call in the Producer and Consumer programs (literally at end of main() ) is: 
        producer(Active_Producer,x)
        consumer(Active_Consumer,x)

        To change the default value of 10 items per program change the value of the x in these functions.
##There is a compact program called ProConThread from where this program was derived
    > That was done with threads and no shared memory is done.