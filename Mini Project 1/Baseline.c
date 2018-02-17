#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


/*
Other include statements may be needed !
*/

struct stat st = {0};
struct dirent **folders;
int isFound, indx = 0;
char *newDirectoryName;

void concIntToString(char *s, int indx);

int main(int argc, char * argv[])   { 

    char * directoryName = argv[1];  /* Directory name to be created */
    newDirectoryName = malloc(strlen(directoryName) + 1);
    strcpy(newDirectoryName, directoryName);

    while (1) {
        isFound = scandir(directoryName, &folders, 0, alphasort);
        if (isFound == -1) {
            mkdir(directoryName, 0700);
            return 0;
        }
        else {
            indx++;
            strcpy(directoryName, newDirectoryName);
            concIntToString(directoryName, indx);
        }
    }
}

void concIntToString(char *s, int indx) {
    char *s2 = malloc( indx + 1 );
    snprintf( s2, indx + 1, "%d", indx );
    strcat(s,s2);
    free(s2);
}