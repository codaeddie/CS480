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