#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


/*
Other include statements may be needed !
*/

struct stat st = {0};
struct dirent **namelist;
int i, n, indx = 0;

int main(int argc, char * argv[])   { 


    char * directoryName = argv[1];  /* Directory name to be created */

    /* Create Directory if it does not exist */
    if (stat(directoryName, &st) == -1) {
        mkdir(directoryName, 0700);
    }
    else {
        n = scandir(".", &namelist, 0, alphasort);
        if (n < 0)
            perror("scandir");
        else {
            /* scan the files that start with the same name
             to get the index to create new duplicate Directory */
            for (i = 0; i < n; i++) {
                char *s = namelist[i]->d_name;
                if (strncmp(s, directoryName, strlen(directoryName)) == 0)
                    indx++;
                free(namelist[i]);
                }
            }
            free(namelist);

            /* create the new duplicate name directory with index */
            int length = snprintf( NULL, 0, "%d", indx );
            char *s2 = malloc( length + 1 );
            snprintf( s2, length + 1, "%d", indx );
            mkdir(strcat(directoryName,s2), 0700);
            free(s2);
        }
}