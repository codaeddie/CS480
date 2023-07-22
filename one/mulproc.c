/*
 This program will fork two child processes running the two programs generated in programming zero in parallel.
 Hint: You mant need to use fork(), exec() family, wait(), exit(), getpid() and etc   ...
 
 Requirements:

 1) Exactly two child processes are created,
 one to run testspecial program and the other is to run testalphabet program;
 2) When a program starts to run, print a message to the output screen
 showing which process (with PID) is running which program, for example:
    "CHILD <16741> process is executing testalphabet program!"
    
 3) When a program is done, the process exits and at the same time,
    a message should be print to the output screen showing which  process (with PID) is done with the program,
    for example:
    "CHILD <16741> process has done with testalphabet program !"
    
 4) The messages should match the real execution orders,
    i.e. when the testspecial program starts/ends,
    the right message should be print out.
    So you need to figure out how to get the starting/ending time of each process.
   
   
 The expected screen print out should be similar as follows:
 
 CHILD <16741> process is executing testalphabet program!
 CHILD <16742> process is executing testspecial program!
, -> 745668
. -> 798072
... ...

CHILD <16742> process has done with testspecial program !
a -> 2973036
b -> 556908
... ...

CHILD <16741> process has done with testalphabet program !
 */

#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define KEY 7764 // secret key

// Function to print a message about a process
void print_message(const char *action, pid_t pid, const char *program) {
    printf("CHILD <%d> process has %s %s program!\n", pid, action, program);
}

int main(void) {

    pid_t wpid, pid;
    int exitStatus;
    // list of executed file
    char executables [2][20] = {
            "testalphabet", "testspecial"
    };

    // create new shared memory with the key, size and permissions
    int shmid = shmget(KEY, sizeof (pid_t) * 2, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        return 1;
    }

    // shared memory is attached to the process
    pid_t * sharedData = shmat(shmid, NULL, 0);
    if (sharedData == (pid_t *) -1) {
        perror("shmat failed");
        return 1;
    }

    // fork two child processes, one for each executable
    for (int i = 0; i < 2; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork failed");
            return 1;
        }
        if (pid == 0) {
            // process has started
            print_message("started executing", getpid(), executables[i]);
            // store the 
            sharedData[i] = getpid();
            // run the executed file
            if (execl(executables[i], executables[i], NULL) == -1) {
                perror("execl failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    // waiting for all children
    while ((wpid = wait(&exitStatus)) > 0) {
        // check if the process is one of the children
        for (int i = 0; i < 2; i++) {
            if (sharedData[i] == wpid) {
                // show a message
                print_message("finished with", wpid, executables[i]);
            }
        }
    }

    // detach the shared memory
    if (shmdt(sharedData) == -1) {
        perror("shmdt failed");
        return 1;
    }

    // destroy the shared memory
    if (shmctl(shmid,IPC_RMID,NULL) == -1) {
        perror("shmctl failed");
        return 1;
    }

    return 0;
}