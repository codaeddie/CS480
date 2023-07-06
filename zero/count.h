/* charcount.h - This header file include type definitions (including function prototypes) and macros 
   used for the programing assignment zero.
*/

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#define ALPHABETSIZE 26     //The total number of alphabetical letter from a - z (case insensitive)
#define SPECIALCHARSIZE 5   // The number of these special characters ','   '.'  ';'  ':'  '!'

// Function prototypes
int isalphabet(int c);
int isspecialchar(int c);
void charcount(char *path, char *filetowrite, long freq[], int size, int (*ischar)(int));
void alphabetlettercount(char *path, char *filetowrite, long alphabetfreq[]);
void specialcharcount(char *path, char *filetowrite,  long charfreq[]);

// Function definitions
int isalphabet(int c){
    return isalpha(tolower(c));
}

int isspecialchar(int c){
    return (c == ',' || c == '.' || c == ':' || c == ';' || c == '!');
}
void charcount(char *path, char *filetowrite, long freq[], int size, int (*ischar)(int)) {
    DIR *dir;
    struct dirent *ent;
    char filepath[1024];

    // Initialize the frequency array
    for (int i = 0; i < size; i++) {
        freq[i] = 0;
    }

    // Open the directory
    if ((dir = opendir(path)) != NULL) {
        // Read each file in the directory
        while ((ent = readdir(dir)) != NULL) {
            // Check if the file is a .txt file
            if (strstr(ent->d_name, ".txt") != NULL) {
                // Construct the full file path
                snprintf(filepath, sizeof(filepath), "%s/%s", path, ent->d_name);

                // Open the file
                FILE *file = fopen(filepath, "r");
                if (file != NULL) {
                    int c;

                    // Read each character in the file
                    while ((c = fgetc(file)) != EOF) {
                        // Check if the character is valid
                        if (ischar(c)) {
                            // Update the frequency array
                            freq[tolower(c) - 'a']++;
                        }
                    }

                    // Close the file
                    fclose(file);
                }
            }
        }

        // Close the directory
        closedir(dir);
    }

    // Write the frequencies to the output file
    FILE *file = fopen(filetowrite, "w");
    if (file != NULL) {
        for (int i = 0; i < size; i++) {
            fprintf(file, "%c -> %ld\n", (char)(i + 'a'), freq[i]);
        }
        fclose(file);
    }
}

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

*/
void alphabetlettercount(char *path, char *filetowrite, long alphabetfreq[]) {
    // Call the charcount function with the is_alphabet function and ALPHABETSIZE
    charcount(path, filetowrite, alphabetfreq, ALPHABETSIZE, isalphabet);
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
void specialcharcount(char *path, char *filetowrite,  long charfreq[]) {
    // Call the charcount function with the is_special function and SPECIALCHARSIZE
    charcount(path, filetowrite, charfreq, SPECIALCHARSIZE, isspecialchar);
}