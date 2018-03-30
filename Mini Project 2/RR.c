#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>

typedef struct process Process;
typedef struct queue Queue;
typedef struct cpu CPU;
unsigned quantumLength;

Process *newProcess(unsigned *pID, unsigned *arrival, unsigned *burst);
void processInfo(Process *p, int *execTime);
Queue *newQueue(unsigned capacity);
int getQueueSizeFromInput(char *filename);
int queueCurrentSize(Queue *q);
void enqueue(Queue *q, Process *process);
void dequeue(Queue *q, CPU *processor);
void readCSV(Queue *q, char *filename);
void RRScheduler(CPU *cpuUNIT, Queue *ready_queue, unsigned *quantumLength);
int computeBurstTime(Process *p, unsigned *quantumLength);

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

void processInfo(Process *p, int *execTime) {

    p->turnAroundTime = *execTime - p->arrivalTime;

    p->waitingTime = p->turnAroundTime - p->completionTime;
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

/*
 * A structure to represent a queue
 */
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

void dequeue(Queue *q, CPU *processor) {
    q->front = 0;
    q->rear = q->currentSize - 1;
    Process *tmpProcess = q->procArr[q->front];

    if (tmpProcess != NULL) {
        tmpProcess->startTime = clock();
        for (size_t i = 0; i < q->currentSize - 1; i++) {
            q->procArr[i] = q->procArr[i + 1];
        }
        q->procArr[q->rear] = tmpProcess;
        tmpProcess->endTime = clock();
    } else {
        for (size_t i = 0; i < q->currentSize - 1; i++) {
            q->procArr[i] = q->procArr[i + 1];
        }
        q->currentSize--;
    }

}

/*********************************************/


/**************** Round Robin ****************/

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

int computeBurstTime(Process *p, unsigned *quantumLength) {
    for (size_t i = *quantumLength; i > 0; i--) {
        if (p->cpuBurstTime != 0) {
            p->cpuBurstTime--;
            p->completionTime++;
        }
    }
    return p->cpuBurstTime;
}

void RRScheduler(CPU *cpuUNIT, Queue *ready_queue, unsigned *quantumLength) {

    Process *newProcess = ready_queue->procArr[ready_queue->front];

    while (newProcess != NULL) {

        if (newProcess->cpuBurstTime > 0) {
            // TODO: Change time
            printf("Time %d:\t P%d Entering quantum\n", 0, newProcess->processID);
            newProcess->cpuBurstTime = computeBurstTime(newProcess, quantumLength);
            time_t end_execution = clock() / CLOCKS_PER_SEC;

            if (newProcess->cpuBurstTime == 0) {

                int execution_time = end_execution - (int) cpuUNIT->timeRunning;
                processInfo(newProcess, &execution_time);


                if (newProcess->contextSwitchCount != 0)
                    cpuUNIT->contextSwitchesTime += newProcess->contextSwitchCount;

                cpuUNIT->waitingTime += newProcess->waitingTime;

                ready_queue->procArr[ready_queue->front] = NULL;

                // TODO: print process Turn around & waiting time
            }
            else {
                newProcess->contextSwitchCount += 1;
            }

            dequeue(ready_queue, cpuUNIT);

            newProcess = ready_queue->procArr[ready_queue->front];
        }
    }
}

/*********************************************/

int main(int argc, char *argv[]) {
    char *FILENAME = "/Users/amrmkayid/Desktop/RoundRobin/Input1.in";
    int queueSize = getQueueSizeFromInput(FILENAME);
    Queue *ready_queue = newQueue(queueSize);
    readCSV(ready_queue, FILENAME);
    printf("Quantum Length: %d\n", quantumLength);

    CPU cpuUNIT = {0, 0, 0, 0};

    cpuUNIT.timeRunning = (int) clock() / CLOCKS_PER_SEC;
    RRScheduler(&cpuUNIT, ready_queue, &quantumLength);

    return 0;

}
