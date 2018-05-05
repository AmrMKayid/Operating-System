#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>

typedef struct page Page;
unsigned numOfFrames = 5;
unsigned numOfPages;

int getFileLines(char *filename);
void readCSV(char *filename);

int main() {

    char *FILENAME = "/Volumes/Life/GUC/Projects/OS/Mini Project 4/PageReplacement//pages.txt";

    numOfPages = getFileLines(FILENAME);
    printf("numOfPages: %i\n", numOfPages);

    readCSV(FILENAME);

    return 0;
}

typedef struct page {
    unsigned pageID;
    unsigned referencedBit;
    unsigned modifiedBit;
} Page;

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

    for (int l = 0; l < k; ++l) {
        char *a[3];
        for (int i = 0; i < 3; ++i) {
            a[i] = strtok(sentences[l], ",");

            while (a[i] != NULL)
                a[++i] = strtok(NULL, ",");

        }
        printf(">>> %s | %s | %s \n", a[0], a[1], a[2]);
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

    lines++;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n')
            lines++;
    }
    fclose(fp);
    return lines;
}