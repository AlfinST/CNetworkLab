# У ада 3 врата: похоть, гнев и жадность.
## To compile it use
    gcc RWShmController.c -o Cntrl -lpthread -lrt
    gcc Reader.c -o Read -lpthread -lrt
    gcc Writer.c -o Write -lpthread -lrt
## How to Run :
    1. Run the Controller with ./Cntrl . Get Initialisation Setup Messages. By default it runs forever.
    2. Run the Writer with ./Write .Get Initialisation Setup Messages. By default it stops after creating 10 items in buffer.
    3. Run the Reader with ./Read  .Get Initialisation Setup Messages. By default it runs forever.

## It's adviced to remove the files in /dev/shm/ as it might affect all programs after a few runs.
> you have 3 hours figure it yourself on how to make it more clean and aviod this. DO IT YOURSELF!
## How to Control the program
    -you can change the frequency of activity by changing the sleep() in reader and writer.By default
        it's activated randomly (can have a max of 3 second gap)

    -To change the default value of 10 items per writer change the value of the x in the While Loop.

## Algorithm for this solution(2nd Reader-Writer Problem) is given in RWAlgo
> Check out wikipedia for more.