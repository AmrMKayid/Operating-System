#include <stdio.h>
#include<stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


/*
Other include statements may be needed !
*/

struct stat st = {0};
struct dirent **namelist;
int i, n, indx = 0;
char *newDirectoryName;

void concIntToString(char *s, int indx);

int main(int argc, char * argv[])   { 


    char * directoryName = argv[1];  /* Directory name to be created */
    newDirectoryName = malloc(strlen(directoryName) + 1);
    strcpy(newDirectoryName, directoryName);

    if (stat(directoryName, &st) == -1) {
        mkdir(directoryName, 0700);
    }
    else {
        n = scandir(".", &namelist, 0, alphasort);
        if (n < 0)
            perror("scandir");
        else {

            for (i = 0; i < n; i++) {
                char *s = namelist[i]->d_name;

                if (strncmp(s, directoryName, strlen(s)) == 0) {
                    indx++;
                    strcpy(directoryName, newDirectoryName);
                    concIntToString(directoryName, indx);
                    // printf("%s\n", directoryName);
                }
                free(namelist[i]);
                }
            }
            free(namelist);

            concIntToString(newDirectoryName, indx);
            // printf("%s\n", newDirectoryName);

            mkdir(newDirectoryName, 0700);

        }
}

void concIntToString(char *s, int indx) {
    char *s2 = malloc( indx + 1 );
    snprintf( s2, indx + 1, "%d", indx );
    strcat(s,s2);
    free(s2);
    // return s;
}