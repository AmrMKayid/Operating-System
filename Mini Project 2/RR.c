#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>

typedef struct process Process;
typedef struct queue Queue;
typedef struct cpu CPU;
unsigned quantumLength;
FILE *outFile;

Process *newProcess(unsigned *pID, unsigned *arrival, unsigned *burst);

Queue *newQueue(unsigned capacity);

int getQueueSizeFromInput(char *filename);

int queueCurrentSize(Queue *q);

void enqueue(Queue *q, Process *process);

void dequeue(Queue *q);

void readCSV(Queue *q, char *filename);

void RRScheduler(Queue *ready_queue, unsigned *quantumLength);

int computeBurstTime(Process *p, unsigned *quantumLength);

void delay(int ms);

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

void dequeue(Queue *q) {
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

    for (int l = 2; l < k; ++l) {
        char *a[4];
        for (int i = 0; i < 4; ++i) {
            a[i] = strtok(sentences[l], ",");

            while (a[i] != NULL)
                a[++i] = strtok(NULL, ",");

        }
//        fprintf(outFile, ">>> %s | %s | %s \n", a[0], a[1], a[2]);
        unsigned processID = atoi(a[0]), arrivalTime = atoi(a[1]), cpuBurstTime = atoi(a[2]);
        enqueue(q, newProcess(&processID, &arrivalTime, &cpuBurstTime));
    }

    fclose(fPointer);
}

void delay(int ms) {
    long halt;
    clock_t current, previous;

    halt = ms * (CLOCKS_PER_SEC / 1000);
    current = previous = clock();

    while ((current - previous) < halt)
        current = clock();
}

int computeBurstTime(Process *p, unsigned *quantumLength) {
    for (size_t i = *quantumLength; i > 0; i--) {
        if (p->cpuBurstTime != 0) {
            p->cpuBurstTime--;
            p->completionTime++;
        }
    }
    delay((int) *quantumLength);
    return p->cpuBurstTime;
}

void RRScheduler(Queue *ready_queue, unsigned *quantumLength) {

    double AverageWaitingTime = 0;
    double AverageTurnaroundTime = 0;

    size_t i = *quantumLength;
    unsigned c = 0;

    Process *newProcess = ready_queue->procArr[ready_queue->front];

    while (newProcess != NULL) {

        if (newProcess->cpuBurstTime > 0) {
            // TODO: Change time
            printf("Time %6d:\t P%d Entering quantum\n", c, newProcess->processID);
            fprintf(outFile, "Time %6d:\t P%d Entering quantum\n", c, newProcess->processID);
            c += (i < newProcess->cpuBurstTime) ? i : newProcess->cpuBurstTime;

            newProcess->cpuBurstTime = computeBurstTime(newProcess, quantumLength);

            if (newProcess->cpuBurstTime == 0) {
                AverageWaitingTime += c - newProcess->completionTime;
                AverageTurnaroundTime += c;
//                fprintf(outFile, "Average Waiting Time= %d\n", AverageWaitingTime);
//                fprintf(outFile, "Average Turnaround Time= %d\n", AverageTurnaroundTime);

                printf("Time %6d:\t P%d Done Turn around: %d Waiting time: %d\n", c, newProcess->processID, c,
                       c - newProcess->completionTime);
                fprintf(outFile, "Time %6d:\t P%d Done Turn around: %d Waiting time: %d\n", c, newProcess->processID, c,
                        c - newProcess->completionTime);

                ready_queue->procArr[ready_queue->front] = NULL;

            } else {
                newProcess->contextSwitchCount += 1;
            }

            dequeue(ready_queue);

            newProcess = ready_queue->procArr[ready_queue->front];
        }
    }


    printf("\nAverage Waiting Time= %f\n", AverageWaitingTime / ready_queue->capacity);
    printf("Average Turnaround Time= %f\n", AverageTurnaroundTime / ready_queue->capacity);

    fprintf(outFile, "\nAverage Waiting Time= %f\n", AverageWaitingTime / ready_queue->capacity);
    fprintf(outFile, "Average Turnaround Time= %f\n", AverageTurnaroundTime / ready_queue->capacity);
}


/*********************************************/

int main(int argc, char *argv[]) {

    char *FILENAME = "/Users/amrmkayid/Desktop/RoundRobin/Input1.in";
    outFile = fopen("Output1-RR.txt", "w");

    int queueSize = getQueueSizeFromInput(FILENAME);
    Queue *ready_queue = newQueue(queueSize);

    readCSV(ready_queue, FILENAME);

    RRScheduler(ready_queue, &quantumLength);


    return 0;

}
