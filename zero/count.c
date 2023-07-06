/* count.c - This header file include type definitions (including function prototypes) and macros
   used for the programing assignment zero.
*/

#include "count.h"

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
            fprintf(file, "%c -> %ld\n", (char)(i + 97), freq[i]);
        }
        fclose(file);
    }
}