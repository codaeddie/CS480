/*
 * alphabetcountmulthreads.c - this file implements the alphabetcountmulthreads function.
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include "count.h"

/**
  The alphabetcountmulthreads function counts the frequency of each alphabet letter (a-z, case insensitive) in all the .txt files under
  directory of the given path and write the results to a file named as filetowrite. Different with programming assignment#0, you need to implement the program using mutithreading.
  
  Input: 
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of each alphabet letter from a - z, which should be already up-to-date;
      num_threads - number of the threads running in parallel to process the files
      
       
  Output: a new file named as filetowrite with the frequency of each alphabet letter written in
  
  Requirements:
1)	Multiple threads are expected to run in parallel to share the workload, i.e. suppose 3 threads to process 30 files, then each thread should process 10 files;
2)	When a thread is created, a message should be print out showing which files this thread will process, for example:
Thread id = 274237184 starts processing files with index from 0 to 10!
3)	When a file is being processed, a message should be print out showing which thread (thread_id = xxx) is processing this file, for example:
Thread id = 265844480 is processing file input_11.txt
4)	When a thread is done with its workload, a message should be print out showing which files this thread has done with work, for example:
      Thread id = 274237184 is done !
5)	The array: long alphabetfreq[ ] should always be up-to-date, i.e. it always has the result of all the threads counted so far.  [You may need to use mutexes to protect this critical region.]


You should have the screen printing should be similar as follows:

 Thread id = 274237184 starts processing files with index from 0 to 10!
 Thread id = 265844480 starts processing files with index from 11 to 22!
 Thread id = 257451776 starts processing files with index from 23 to 31!

 Thread id = 265844480 is processing file input_11.txt
 Thread id = 257451776 is processing file input_22.txt
 Thread id = 274237184 is processing file input_00.txt
  

 Thread id = 274237184 is done !
 Thread id = 265844480 is done !
 Thread id = 257451776 is done !

 The results are counted as follows:
 a -> 2973036
 b -> 556908
 c -> 765864
 ... ...
*/

// Mutex for updating the global alphabetfreq array
pthread_mutex_t lock;

// Struct for passing data to threads
typedef struct {
    char *path;
    char *filetowrite;
    long alphabetfreq[26];
    int start_index;
    int end_index;
} thread_data_t;

// Helper function for processing a single file
void process_file(char *filename, thread_data_t *data) {
    FILE *file;
    int ch;

    // Open the file and count the frequency of each alphabet letter
    file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }

    // alphabetfreq[0]: the frequency of 'a' has
    // up-to-date frequencies of alphabet letters counted
    // so far. If letter is upper case, convert it to lower
    while ((ch = fgetc(file)) != EOF) {
        if (isalpha(ch)) {
            ch = tolower(ch);
            data->alphabetfreq[ch - 'a']++;
        }
    }

    // Close the fetched file
    fclose(file);
}

// Function for processing files
void *process_files(void *arg) {
    // Cast the argument to thread_data_t
    thread_data_t *data = (thread_data_t *)arg;

    // Print start message
    printf("Thread id = %lu starts processing files with index from %d to %d!\n", pthread_self(), data->start_index, data->end_index);

    // Process files...
    DIR *dir;
    struct dirent *readDir;

    // Check if the path is valid and opened successfully
    dir = opendir(data->path);
    if (dir == NULL) {
        return NULL;
    }

    // Read all files in the directory one by one
    while ((readDir = readdir(dir)) != NULL) {
        size_t len = strlen(readDir->d_name);

        // Find all the files ending with .txt and store in list
        if (len > 4 && strcmp(readDir->d_name + len - 4, ".txt") == 0) {
            char filepath[1024];
            sprintf(filepath, "%s/%s", data->path, readDir->d_name);

            // Print message indicating which file is being processed
            printf("Thread id = %lu is processing file %s\n", pthread_self(), readDir->d_name);

            // Process the file
            process_file(filepath, data);
        } //end finding .txt files
    } //end reading all files

    // Lock the mutex before updating the global alphabetfreq array
    pthread_mutex_lock(&lock);

    // Update the global alphabetfreq array with the counts from the local array
    for (int i = 0; i < 26; i++) {
        data->alphabetfreq[i] += data->alphabetfreq[i];
    }

    // Unlock the mutex after updating the alphabetfreq array
    pthread_mutex_unlock(&lock);

    // Print end message
    printf("Thread id = %lu is done!\n", pthread_self());

    return NULL;
}

// Helper function for writing the results to a file
void write_results(char *filetowrite, long alphabetfreq[]) {
    FILE *file;
    int ch;

    // Open the file for writing
    file = fopen(filetowrite, "w");
    if (file == NULL) {
        return;
    }

    // iterate through alphabetfreq and write the frequency
    // of each alphabet letter in the output file
    for (ch = 0; ch < 26; ch++) {
        fprintf(file, "%c -> %ld\n", 'a' + ch, alphabetfreq[ch]);
    }

    fclose(file);
}

int count_files(char *path) {
    DIR *dir;
    struct dirent *readDir;
    int count = 0;

    // Open the directory
    dir = opendir(path);
    if (dir == NULL) {
        return -1;
    }

    // Count .txt files
    while ((readDir = readdir(dir)) != NULL) {
        size_t len = strlen(readDir->d_name);
        if (len > 4 && strcmp(readDir->d_name + len - 4, ".txt") == 0) {
            count++;
        }
    }

    closedir(dir);
    return count;
}

void alphabetcountmulthreads(char *path, char *filetowrite, long alphabetfreq[], int num_threads) {
    // Initialize the mutex
    pthread_mutex_init(&lock, NULL);

    // Create an array of threads
    pthread_t threads[num_threads];

    // Create an array of thread_data_t structs
    thread_data_t thread_data[num_threads];

    // Calculate the total number of files
    int total_files = count_files(path);
    int files_per_thread = total_files / num_threads;

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        // Initialize the thread_data struct
        thread_data[i].path = path;
        thread_data[i].filetowrite = filetowrite;

        // Assign a portion of the files to this thread
        thread_data[i].start_index = i * files_per_thread;
        thread_data[i].end_index = (i + 1) * files_per_thread;

        // In the last thread, we handle the remaining files
        if (i == num_threads - 1) thread_data[i].end_index = total_files;

        // Create the thread
        pthread_create(&threads[i], NULL, process_files, &thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Write results to file
    write_results(filetowrite, alphabetfreq);

    // Destroy the mutex
    pthread_mutex_destroy(&lock);
}