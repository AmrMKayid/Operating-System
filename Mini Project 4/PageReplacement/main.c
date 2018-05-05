#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>

int getFileLines(char *filename);
void readCSV(char *filename);

int main() {

    char *FILENAME = "/Users/amrmkayid/Desktop/PageReplacement/pages.txt";
    readCSV(FILENAME);

    return 0;
}

void readCSV(char *filename) {

    FILE *fPointer;
    fPointer = fopen(filename, "r");

    int lines = getFileLines(filename) + 2;

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