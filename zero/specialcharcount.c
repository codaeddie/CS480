/*
 * specialcharcount.c - this file implements the specialcharcount function.
 */


#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include "count.h"

void specialcharcount(char *path, char *filetowrite, long charfreq[])
{
    DIR *dir;
    struct dirent *readDir;
    FILE *file;
    int ch, i;
    char specialchars[] = {',', '.', ':', ';', '!'};

    // Check if the path is valid and opened successfully
    dir = opendir(path);
    if (dir == NULL) {
        return;
    }

    // Read all files in the directory one by one
    while ((readDir = readdir(dir)) != NULL) {
        size_t len = strlen(readDir->d_name);

        // Check if the file is a .txt file
        // store all the .txt files in filelist
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
                for (i = 0; specialchars[i] != '\0'; i++) {
                    if (ch == specialchars[i]) {
                        charfreq[i]++;
                        break;
                    }
                }
            } //end reading the file

            // Close the fetched file
            fclose(file);
        } //end finding .txt files
    } // end reading all files

    // Write the result in the output file
    file = fopen(filetowrite, "w");
    if (file == NULL) {
        return;
    }

    // iterate through alphabetfreq and write the frequency
    // of each alphabet letter in the output file
    for (i = 0; specialchars[i] != '\0'; i++) {
        fprintf(file, "%c -> %ld\n", specialchars[i], charfreq[i]);
    }

    // HOW'S MY CODING?
    // PLEASE CALL TO LEAVE A REVIEW
    // 1-800-DEV-HELP
    fclose(file);
    closedir(dir);
}


/**
  The specialcharcount function counts the frequency of the following 5 special characters:
  ','   '.'   ':'    ';'    '!'
   
  in all the .txt files under directory of the given path and write the results to a file named as filetowrite.
  
  Input: 
  
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of the above 5 special characters
      
      charfreq[0]: the frequency of ','
      charfreq[1]: the frequency of '.'
      charfreq[2]: the frequency of ':'
      charfreq[3]: the frequency of ';'
      charfreq[4]: the frequency of '!'

  
  Output: a new file named as filetowrite with the frequency of the above special characters written in
  
  Steps recommended to finish the function:
  1) Find all the files ending with .txt and store in filelist.
  2) Read all files in the filelist one by one and count the frequency of each alphabet letter only (a - z). The array 
  long alphabetfreq[] always has the up-to-date frequencies of alphabet letters counted so far. If the letter is upper case, convert
  it to lower case first. 
  3) Write the result in the output file: filetowrite in following format: 
  
     character -> frequency
     
     example:
     , -> 20
     . -> 11
     : -> 20
     ; -> 11
     ! -> 12     
     
  Assumption:  
  1) You can assume there is no sub-directory under the given path so you don't have to search the files 
  recursively.    
  2) Only .txt files are counted and other types of files should be ignored.
  
*/