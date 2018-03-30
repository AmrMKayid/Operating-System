#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>

typedef struct process Process;
typedef struct queue Queue;
typedef struct cpu CPU;
unsigned quantumLength;

Process *newProcess(unsigned *pID, unsigned *arrival, unsigned *burst);
Queue *newQueue(unsigned capacity);
int getQueueSizeFromInput(char *filename);
int queueCurrentSize(Queue *q);
void enqueue(Queue *q, Process *process);
void readCSV(Queue *q, char *filename);

/**************** Processes ****************/
/*
 * A structure to represent a Process
 */
typedef struct process {
    unsigned processID;
    unsigned arrivalTime;
    unsigned cpuBurstTime;
    unsigned startTime;
    unsigned endTime;
    unsigned waitingTime;
    unsigned completionTime;
    unsigned turnAroundTime;
    unsigned contextSwitchCount;
} Process;

Process *newProcess(unsigned *pID, unsigned *arrival, unsigned *burst) {
    Process *p;
    p = malloc(sizeof(Process));
    p->processID = *pID;
    p->arrivalTime = *arrival;
    p->cpuBurstTime = *burst;
    p->waitingTime = 0;
    p->completionTime = 0;
    p->turnAroundTime = 0;
    return p;
}

/*********************************************/

/****************** CPU *********************/

typedef struct cpu {
    unsigned waitingTime;
    unsigned completionTime;
    unsigned contextSwitchesTime;
    time_t timeRunning;
} CPU;


/*********************************************/


/**************** ReadyQueue ****************/

typedef struct queue {
    Process *procArr[1000];
    int front, rear, currentSize;
    unsigned capacity;
} Queue;

Queue *newQueue(unsigned capacity) {
    Queue *q = malloc(sizeof(Queue));
    q->capacity = capacity;
    q->front = q->currentSize = 0;
    q->rear = capacity - 1;
//    q->procArr = malloc(q->capacity * sizeof(Process));
    return q;
}

int getQueueSizeFromInput(char *filename) {
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
    return lines - 2;
}

int queueCurrentSize(Queue *q) {
    return q->currentSize;
}

void enqueue(Queue *q, Process *process) {
    unsigned currentSize = queueCurrentSize(q);

    if (q->procArr[currentSize] == NULL && currentSize < q->capacity) {
        q->procArr[currentSize] = process;
        q->currentSize++;
    }
}

/*********************************************/

void readCSV(Queue *q, char *filename) {

    FILE *fPointer;
    fPointer = fopen(filename, "r");

    int lines = getQueueSizeFromInput(filename) + 2;

    char singleLine[1024];


    int k = 0;

    char sentences[lines][1024];

    while (!feof(fPointer)) {
        fgets(singleLine, 1024, fPointer);
        strcpy(sentences[k], singleLine);
        k++;
    }

    quantumLength = atoi(sentences[0]);

    for(int l = 2; l < k; ++l) {
        char *a[4];
        for(int i = 0; i < 4; ++i) {
            a[i] = strtok(sentences[l], ",");

            while(a[i]!=NULL)
                a[++i] = strtok(NULL, ",");

        }
//        printf(">>> %s | %s | %s \n", a[0], a[1], a[2]);
        unsigned processID = atoi(a[0]), arrivalTime = atoi(a[1]), cpuBurstTime = atoi(a[2]);
        enqueue(q, newProcess(&processID, &arrivalTime, &cpuBurstTime));
    }

    fclose(fPointer);
}


int main(int argc, char *argv[]) {
    char *FILENAME = "/Users/amrmkayid/Desktop/RoundRobin/Input1.in";
    int queueSize = getQueueSizeFromInput(FILENAME);
    Queue *ready_queue = newQueue(queueSize);
    readCSV(ready_queue, FILENAME);
    printf("Quantum Length: %d\n", quantumLength);

    return 0;

}
