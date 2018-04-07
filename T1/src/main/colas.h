#include <stdlib.h>
#include <stdio.h>
#include "procesos.h"

#define TRUE  1
#define FALSE	0


typedef struct Queue {    //cola para MLFQ
    Proceso *head;
    Proceso *tail;
    int size;
		int prioridad;
		struct Queue *next;
} Queue;

Queue *ConstructQueue();
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, Proceso *item);
Proceso *Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);
