#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

typedef struct page Page;

unsigned numOfFrames;
unsigned numOfPages;

int getFileLines(char *filename);

void readCSV(char *filename);

Page *newPage(int accessTime, int pageID, char *bitType);

void printPage(Page *p);


/**************** Page ****************/
typedef struct page {
    int accessTime;
    int pageID;
    int referencedBit;
    int modifiedBit;
    char *bitType;
} Page;

Page *newPage(int accessTime, int pageID, char *bitType) {
    Page *p;
    p = malloc(sizeof(Page));
    p->accessTime = accessTime;
    p->pageID = pageID;
    p->bitType = bitType;
    p->referencedBit = 0;
    p->modifiedBit = 0;

    if (strcmp(p->bitType, "R") == 13)
        p->referencedBit = 1;

    if (strcmp(p->bitType, "W") > 0)
        p->modifiedBit = 1;

    return p;
}

void printPage(Page *p) {
    printf("%2u %2u %2u %2u %s\n", p->accessTime, p->pageID, p->referencedBit, p->modifiedBit, p->bitType);
}

/*************************************/


bool isFound(Page listOfPages[5], Page *p, int size) {
    for (int i = 0; i < size; ++i) {
        if (listOfPages[i].pageID == p->pageID)
            return true;
    }
    return false;
}

void pop(Page listOfPages[5], int size, int index) {
    for (int i = 0; i < size; ++i) {
        listOfPages[i] = listOfPages[i + 1];
    }
    index--;
}

void readCSV(char *filename) {

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

    Page *listOfPages[numOfFrames];
    int index = 0;

    for (int l = 0; l < k; ++l) {
        char *a[3];
        for (int i = 0; i < 3; ++i) {
            a[i] = strtok(sentences[l], ",");

            while (a[i] != NULL)
                a[++i] = strtok(NULL, ",");

        }
        int accessTime = atoi(a[0]), pageID = atoi(a[1]);
        char *bitType = a[2];


        /*
         * FIFO page replacement algorithm
         */

        if (index < numOfFrames) {
            Page *p = newPage(accessTime, pageID, bitType);
            if (!isFound(&listOfPages, p, numOfFrames))
                listOfPages[index++] = p;
        } else {
            pop(&listOfPages, numOfFrames, index);
        }
    }

    fclose(fPointer);
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

int main() {

    char *FILENAME = "/Volumes/Life/GUC/Projects/OS/Mini Project 4/PageReplacement//pages.txt";

    numOfPages = getFileLines(FILENAME);
    printf("numOfPages: %i\n", numOfPages);

    numOfFrames = 5;

    readCSV(FILENAME);

    return 0;
}