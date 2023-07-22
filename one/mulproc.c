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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pid1, pid2;

    // Fork the first child process.
    pid1 = fork();
    if (pid1 < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    if (pid1 == 0) { // Child process
        printf("CHILD <PID: %d> process is executing testalphabet program!\n", getpid());
        execl("./testalphabet", "testalphabet", (char *) NULL);
        // If execl returns, there was an error.
        perror("execl failed");
        return 1;
    }

    // Fork the second child process.
    pid2 = fork();
    if (pid2 < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    }

    if (pid2 == 0) { // Child process
        printf("CHILD <PID: %d> process is executing testspecial program!\n", getpid());
        execl("./testspecial", "testspecial", (char *) NULL);
        // If execl returns, there was an error.
        perror("execl failed");
        return 1;
    }

    // Wait for the second child process to finish.
    waitpid(pid2, NULL, 0);
    printf("CHILD <PID: %d> process has done with testspecial program!\n", pid2);

    // Wait for the first child process to finish.
    waitpid(pid1, NULL, 0);
    printf("CHILD <PID: %d> process has done with testalphabet program!\n", pid1);

    return 0;
}