#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>

typedef struct page Page;
typedef struct node Node;

unsigned numOfFrames = 5;
unsigned numOfPages;

int getFileLines(char *filename);

void readCSV(Node *start, char *filename);

void push(Node **head_ref, void *new_data, size_t data_size);

void printList(Node *node, void (*fptr)(void *));

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


/******* LinkedList of Pages ********/

typedef struct node {
    // Any data type can be stored in this node
    void *data;

    struct node *next;
} Node;

void push(Node **head_ref, void *new_data, size_t data_size) {
    // Allocate memory for node
    Node *new_node = (Node *) malloc(sizeof(Node));

    new_node->data = malloc(data_size);
    new_node->next = (*head_ref);

    // Copy contents of new_data to newly allocated memory.
    // Assumption: char takes 1 byte.
    int i;
    for (i = 0; i < data_size; i++)
        *(char *) (new_node->data + i) = *(char *) (new_data + i);

    // Change head pointer as new node is added at the beginning
    (*head_ref) = new_node;
}

void printList(Node *node, void (*fptr)(void *)) {
    while (node != NULL) {
        (*fptr)(node->data);
        node = node->next;
    }
}

/*************************************/

void readCSV(Node *start, char *filename) {

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

    unsigned page_size = sizeof(Page);

    for (int l = 0; l < k; ++l) {
        char *a[3];
        for (int i = 0; i < 3; ++i) {
            a[i] = strtok(sentences[l], ",");

            while (a[i] != NULL)
                a[++i] = strtok(NULL, ",");

        }
        int accessTime = atoi(a[0]), pageID = atoi(a[1]);
        char *bitType = a[2];
        push(&start, newPage(accessTime, pageID, bitType), page_size);

    }

    printList(start, printPage);
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

    Node *start = NULL;

    readCSV(start, FILENAME);

    printList(start, printPage);

    return 0;
}

/*
#include<stdio.h>

int main() {
    int k, nop, frame_size, i, j, temp = 100, page_fault = 0;
    int page_name[20], frame[20];
    int m, n = 0, count = 0;
    printf("Enter the No. of pages\t");
    scanf("%d", &nop);
    printf("Enter the frame size\t");
    scanf("%d", &frame_size);
    printf("Enter page name\n");
    for (i = 0; i < nop; i++)
        scanf("%d", &page_name[i]);
    for (i = 0; i < frame_size; i++)
        frame[i] = 0;
    for (i = 0, j = 0; i < frame_size; i++, j++) {
        frame[j] = page_name[i];
        page_fault++;
        printf("\npage\t%d\t", page_name[i]);
        printf("Frame\t");
        for (k = 0; k < frame_size; k++)
            printf(" %d", frame[k]);
        printf("\tPage fault till now:\t");
        printf("%d", page_fault);
    }
    j = 0;

    for (m = i; m < nop; m++) {
        if (page_name[m] == frame[j]) {
            j++;
            count++;
            printf("\npage\t%d\t", page_name[m]);
            printf("Frame\t");
            for (k = 0; k < frame_size; k++)
                printf(" %d", frame[k]);
            printf("\tPage fault till now:\t");
            printf("%d", page_fault);
        } else {
            frame[j] = page_name[m];
            page_fault++;
            j++;
            count++;
            printf("\npage\t%d\t", page_name[m]);
            printf("Frame\t");
            for (k = 0; k < frame_size; k++)
                printf(" %d", frame[k]);
            printf("\tPage fault till now:\t");
            printf("%d", page_fault);

        }
        if (count == frame_size)
            j = 0;
    }
    return 0;
}
 */