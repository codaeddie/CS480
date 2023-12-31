/*
 * alphabetcount.c - this file implements the alphabetlettercount function.
 */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include "count.h"
/**
  The alphabetlettercount function counts the frequency of each alphabet letter (a-z, case insensitive) in all the .txt files under
  directory of the given path and write the results to a file named as filetowrite.
  
  Input: 
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of each alphabet letter from a - z:
      alphabetfreq[0]: the frequency of 'a'
      alphabetfreq[1]: the frequency of 'b'
         ... ...
      alphabetfreq[25]:the frequency of 'z'

  
  Output: a new file named as filetowrite with the frequency of each alphabet letter written in
  
  Steps recommended to finish the function:
  1) Find all the files ending with .txt and store in filelist.
  2) Read all files in the filelist one by one and count the frequency of each alphabet letter only (a - z). The array 
  long alphabetfreq[] always has the up-to-date frequencies of alphabet letters counted so far. If the letter is upper case, convert
  it to lower case first. 
  3) Write the result in the output file: filetowrite in following format: 
  
     letter -> frequency
     
     example:
     a -> 200
     b -> 101
     ... ...
     
  Assumption:  
  1) You can assume there is no sub-directory under the given path so you don't have to search the files 
  recursively.    
  2) Only .txt files are counted and other types of files should be ignored.
  
*/

void alphabetlettercount(char *path, char *filetowrite, long alphabetfreq[])
{
    DIR *dir;
    struct dirent *readDir;
    FILE *file;
    int ch;

    // Check if the path is valid and opened successfully
    dir = opendir(path);
    if (dir == NULL) {
        return;
    }

    // Read all files in the directory one by one
    while ((readDir = readdir(dir)) != NULL) {
        size_t len = strlen(readDir->d_name);

        // Find all the files ending with .txt and store in list
        if (len > 4 && strcmp(readDir->d_name + len - 4, ".txt") == 0) {
            char filepath[1024];
            sprintf(filepath, "%s/%s", path, readDir->d_name);

            // Open the file and count the frequency of each alphabet letter
            file = fopen(filepath, "r");
            if (file == NULL) {
                continue;
            }

            // alphabetfreq[0]: the frequency of 'a' has
            // up-to-date frequencies of alphabet letters counted
            // so far. If letter is upper case, convert it to lower
            while ((ch = fgetc(file)) != EOF) {
                if (isalpha(ch)) {
                    ch = tolower(ch);
                    alphabetfreq[ch - 'a']++;
                }
            }

            // Close the fetched file
            fclose(file);
        } //end finding .txt files
    } //end reading all files

    // Write the result in the output file
    file = fopen(filetowrite, "w");
    if (file == NULL) {
        return;
    }

    // iterate through alphabetfreq and write the frequency
    // of each alphabet letter in the output file
    for (ch = 0; ch < 26; ch++) {
        fprintf(file, "%c -> %ld\n", 'a' + ch, alphabetfreq[ch]);
    }

    // HOW'S MY CODING? 
    // PLEASE CALL TO LEAVE A REVIEW
    // 1-800-DEV-HELP
    fclose(file);
    closedir(dir);
}