#include <stdlib.h>
#include <stdio.h>
#include "procesos.h"

#define TRUE  1
#define FALSE	0


typedef struct Queue {    //cola para procesos
    Proceso *head;
    Proceso *tail;
    int size;
		int prioridad;
		struct Queue *next;
} Queue;

typedef struct Queue_Queue {    //cola para MLFQ
    Queue *head;
    Queue *tail;
    int size;
		struct Queue *next;
} Queue_Queue;

Queue *ConstructQueue();
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, Proceso *item);
int Queue_Enqueue(Queue_Queue *pQueue, Queue *item);
Proceso *Dequeue(Queue *pQueue);
int isEmpty(Queue pQueue);
int Print_Queue(Queue *pQueue);
int Ordered_Enqueue(Queue *pQueue, Proceso *item);

Queue_Queue *ConstructMLFQueue(int k);
int Queue_Print_Queue(Queue_Queue *pQueue);

Proceso *Born(Queue pQueue, int time);

int Ejecutar_proceso(Queue_Queue *pQueue, int quantum);
