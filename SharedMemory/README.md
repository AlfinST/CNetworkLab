# Эта программа была боль в тылу для реализации
## To compile it use:
    gcc ShmClientz.c  -o Node -lpthread -lpthread

    gcc ShmServer.c -0 Host -lpthread -lrt
## How to Run:

    1.For single process incrementor:
        On running the "Host" first you get a bunch of initial messages about stuff getiing setup and initaialted
        Then run the "Node". 
        In the code for the Client/"Node" it's shown that some messages like the Host are present .
            Those messages in Client need not appear when you run the program, but it still runs. (Not that you will bother with that (:p))
    2.For mltiple process incrementor:
        Run the host.
        Run multiple "Node" in different terminals
        Watch the magic


## Some common error we run into 
    1.Permission    
    2.Already Exists    
    3. ...  

        1. Is already solved by using appropiate flags like S_IRWXU which is equivalent to 0666(permission to read ,wite and execute to everyone).
            Feel free to change the falg 0660 to any in the semaphors used in the programs. 

        2. using the O_EXCL flag helps ... kinda ..... maybe . OK, i dont know exactly if it helps cause I just go to /dev/shm/ and delete all the 
            files present there to make sure that the semaphor and shared_memoy objects/files present are gone.Thus avoiding the error.
            I read the using sem_unlink() might help/is a good practice. 
        
        3.The error handeling could have been done better but its waay too much work.
# Its adviced to remove the files in /dev/shm/ as it might affect the server program after a few runs.
## How to control the program ?
    -The variable x is used in the Client/"Node" program change to determine the number of times that program should increment the Number.

    -There is a if condition in the Server/"Host" to determine when the program should terminate.

    -A sleep() is given in the Client/"Node" program to determine how frequently it increment. Its given as 1 (increments every 1 seconds)

## What is race condition in Shared Memory ?
    When a shared memory is accessed by 2 or more processes and try to change the same value , A race condition is acheved.
    Eg: 2 processes A,B ;
        both access a shared variable C;
        A changes to 10;
        B changes to 20;
            if A tried to access C now it will get the wrong value 20 insted of 10 which it assigned.

# There is a compact program called SemExample.c which is a Compact version of these 2 programs using threads.
>This compact program is done using System V (system 5) not POSIX. Just use it as a reference.