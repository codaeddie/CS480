Please answer the following questions regarding your programming assignment#2.


1) Please describe your design of your implementation.






2) Please execute your program by varying thread quantities: {3, 4, 6, 8, 10, 12}, measure the execution times and document your measurement below.

(Note: To ensure precise timing for each configuration, it may be necessary to conduct multiple experiments and calculate the average.)





       

3) Do the observed trends in the measured execution times align with your initial expectations? Please provide a justification for your response.







4) Is it possible to improve your design to achieve a greater speedup for your program?  If so, what are the possible improvements you can make to your design? 

 If not, why you think your current design is optimal?



-----------------------Notes-----------------------
-----------------------Notes-----------------------

#include <pthread.h>
#include <stdio.h>
#include <dirent.h>
#include "count.h"

// Mutex for updating the global alphabetfreq array
pthread_mutex_t lock;

// Struct for passing data to threads
typedef struct {
    char *path;
    char *filetowrite;
    long *alphabetfreq;
    int start_index;
    int end_index;
} thread_data_t;

// Function for processing files
void *process_files(void *arg) {
    // Cast the argument to thread_data_t
    thread_data_t *data = (thread_data_t *)arg;

    // Print start message
    printf("Thread id = %lu starts processing files with index from %d to %d!\n", pthread_self(), data->start_index, data->end_index);

    // Process files...
    // Open each file, read it character by character, and update a local alphabetfreq array based on the characters it finds

    // Lock the mutex, update the global alphabetfreq array with the local counts, and unlock the mutex

    // Print end message
    printf("Thread id = %lu is done!\n", pthread_self());

    return NULL;
}

void alphabetcountmulthreads(char *path, char *filetowrite, long alphabetfreq[], int num_threads) {
    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);

    // Create an array of threads
    pthread_t threads[num_threads];

    // Create an array of thread_data_t structs
    thread_data_t thread_data[num_threads];

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        // Initialize the thread_data struct
        thread_data[i].path = path;
        thread_data[i].filetowrite = filetowrite;
        thread_data[i].alphabetfreq = alphabetfreq;
        // Assign a portion of the files to this thread
        thread_data[i].start_index = i * num_files / num_threads;
        thread_data[i].end_index = (i + 1) * num_files / num_threads - 1;

        // Create the thread
        pthread_create(&threads[i], NULL, process_files, &thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Write results to file...
    // Open the file for writing, loop over the alphabetfreq array, and write each frequency to the file

    // Destroy the mutex
    pthread_mutex_destroy(&lock);
}


___________________________________________________
#include <pthread.h>
#include <stdio.h>
#include <dirent.h>
#include "count.h"

// Mutex for updating the global alphabetfreq array
pthread_mutex_t lock;

// Struct for passing data to threads
typedef struct {
    char *path;
    char *filetowrite;
    long *alphabetfreq;
    int start_index;
    int end_index;
} thread_data_t;

// Function for processing files
void *process_files(void *arg) {
    // Cast the argument to thread_data_t
    thread_data_t *data = (thread_data_t *)arg;

    // Print start message
    printf("Thread id = %lu starts processing files with index from %d to %d!\n", pthread_self(), data->start_index, data->end_index);

    // Process files...

    // Print end message
    printf("Thread id = %lu is done!\n", pthread_self());

    return NULL;
}

void alphabetcountmulthreads(char *path, char *filetowrite, long alphabetfreq[], int num_threads) {
    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);

    // Create an array of threads
    pthread_t threads[num_threads];

    // Create an array of thread_data_t structs
    thread_data_t thread_data[num_threads];

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        // Initialize the thread_data struct
        thread_data[i].path = path;
        thread_data[i].filetowrite = filetowrite;
        thread_data[i].alphabetfreq = alphabetfreq;
        // Assign a portion of the files to this thread
        thread_data[i].start_index = i * num_files / num_threads;
        thread_data[i].end_index = (i + 1) * num_files / num_threads - 1;

        // Create the thread
        pthread_create(&threads[i], NULL, process_files, &thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Write results to file...

    // Destroy the mutex
    pthread_mutex_destroy(&lock);
}

___________________________________________________
Design:

Thread Creation: We will create num_threads threads using the pthread_create function. Each thread will be assigned a portion of the files in the directory to process.

File Processing Function: Each thread will run a separate function that processes a subset of the files in the directory. The function will open each file, read it character by character, and update a local alphabetfreq array based on the characters it finds.

Mutex for Thread Safety: To ensure that updates to the global alphabetfreq array are thread-safe, we'll use a mutex. Each thread will lock the mutex before updating the global alphabetfreq array with its local counts and unlock it afterwards.

Printing Messages: The file processing function will print messages when it starts processing a file and when it finishes processing a file. These messages will include the thread's ID and the name of the file being processed.

Waiting for Threads to Finish: After creating the threads, the alphabetcountmulthreads function will wait for all of them to finish using the pthread_join function.

Writing Results to File: Once all threads have finished, the alphabetcountmulthreads function will write the contents of the alphabetfreq array to the filetowrite file.
___________________________________________________
Thread Creation: The function will need to create num_threads threads. This can be done using the pthread_create function in a loop that runs num_threads times. Each thread will need to run a separate function that performs the file processing.

File Processing Function: This function will be run by each thread. It will need to take a portion of the files in the directory (based on the thread's index) and process them. The processing will involve opening each file, reading it character by character, and updating the alphabetfreq array based on the characters it finds. To ensure that updates to the alphabetfreq array are thread-safe, you'll need to use a mutex to lock the array before updating it and unlock it afterwards.

Printing Messages: The file processing function should also print messages when it starts processing a file and when it finishes processing a file. These messages should include the thread's ID (which can be obtained using pthread_self) and the name of the file being processed.

Waiting for Threads to Finish: After creating the threads, the alphabetcountmulthreads function will need to wait for all of them to finish before it can continue. This can be done using the pthread_join function in a loop that runs num_threads times.

Writing Results to File: Once all threads have finished, the alphabetcountmulthreads function can write the contents of the alphabetfreq array to the filetowrite file. This will involve opening the file for writing, looping over the alphabetfreq array, and writing each frequency to the file.
___________________________________________________
The difference from Project Zero is that this function is expected to be implemented using multithreading.


The function takes four arguments:

path: a pointer to a char string specifying the path of the directory.
filetowrite: a pointer to a char string specifying the file where results should be written in.
alphabetfreq: a pointer to a long array storing the frequency of each alphabet letter from a - z, which should be already up-to-date.
num_threads: the number of threads running in parallel to process the files.
The function is expected to create multiple threads to share the workload. For example, if there are 3 threads to process 30 files, then each thread should process 10 files.

When a thread is created, a message should be printed showing which files this thread will process. When a file is being processed, a message should be printed showing which thread is processing this file. When a thread is done with its workload, a message should be printed showing which files this thread has done with work.

The alphabetfreq array should always be up-to-date, i.e., it always has the result of all the threads counted so far. This may require the use of mutexes to protect this critical region.




