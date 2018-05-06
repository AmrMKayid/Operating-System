#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct page {
    int accessTime;
    int pageID;
    int referencedBit;
    int modifiedBit;
} Page;

Page *newPage(int accessTime, int pageID, char *bitType) {
    Page *p;
    p = malloc(sizeof(Page));
    p->accessTime = accessTime;
    p->pageID = pageID;
    p->referencedBit = 0;
    p->modifiedBit = 0;

    if (strcmp(bitType, "R") == 13)
        p->referencedBit = 1;

    if (strcmp(bitType, "W") > 0)
        p->modifiedBit = 1;

    return p;
}

void printPage(Page *p) {
    printf("%2u (pageID: %2u) %2u %2u\n", p->accessTime, p->pageID, p->referencedBit, p->modifiedBit);
}


int getFileLines(char *filename) {
    // count the number of lines in the file called filename
    FILE *fp = fopen(filename, "r");
    int ch = 0;
    int lines = 0;

    if (fp == NULL)
        return 0;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n')
            lines++;
    }
    fclose(fp);
    return lines;
}

void fifoAlgorithm(char *filename) {

    FILE *fPointer;
    fPointer = fopen(filename, "r");

    int lines = getFileLines(filename);

    char singleLine[1024];

    int k = 0;

    char sentences[lines][1024];

    while (!feof(fPointer)) {
        fgets(singleLine, 1024, fPointer);
        strcpy(sentences[k], singleLine);
        k++;
    }

    Page *listOfPages[5];
    int counter = 0;

    for (int l = 0; l < k - 1; ++l) {
        char *a[3];
        for (int i = 0; i < 3; ++i) {
            a[i] = strtok(sentences[l], ",");

            while (a[i] != NULL)
                a[++i] = strtok(NULL, ",");

        }
        int accessTime = atoi(a[0]), pageID = atoi(a[1]);
        char *bitType = a[2];

        Page *p = newPage(accessTime, pageID, bitType);
//        printPage(p);

        /*
         * FIFO Algorithm
         */
        bool found = false;
        if (counter < 5) {
            for (int i = 0; i < counter; ++i) {
                if (listOfPages[i]->pageID == p->pageID)
                    found = true;
            }

            if (!found) {
                listOfPages[counter++] = p;
                printf("Added new Page to the list: ");
                printPage(p);
            } else {
                printf("This Page is found in the list: ");
                printPage(p);
            }

        } else {

            for (int i = 0; i < counter; ++i) {
                if (listOfPages[i]->pageID == p->pageID)
                    found = true;
            }

            if (!found) {
                printf("Removing the first Page from the list: ");
                printPage(listOfPages[0]);
                for (int i = 0; i < counter; ++i) {
                    listOfPages[i] = listOfPages[i + 1];
                }


                listOfPages[counter - 1] = p;
                printf("Added new Page to the list: ");
                printPage(p);
            } else {
                printf("This Page is found in the list: ");
                printPage(p);
            }
        }

    }

    fclose(fPointer);
}


int main() {

    char *FILENAME = "/Volumes/Life/GUC/Projects/OS/Mini Project 4/PageReplacement/pages.txt";

    fifoAlgorithm(FILENAME);

    return 0;
}