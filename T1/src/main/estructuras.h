#include <stdlib.h>
#include <stdio.h>

#define TRUE  1
#define FALSE	0

struct Process
{
	int PID;
  int prioridad;
	char nombre;
	int estado; // 1=running, 2=ready, 3=finished
  struct Process *next;
};
typedef struct Process Proceso;

typedef struct Queue {    //cola para MLFQ
    Proceso *head;
    Proceso *tail;
    int size;
} Queue;

Queue *ConstructQueue();
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, Proceso *item);
Proceso *Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);

Queue *ConstructQueue() {
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void DestructQueue(Queue *queue) {
    Proceso *pN;
    while (!isEmpty(queue)) {
        pN = Dequeue(queue);
        free(pN);
    }
    free(queue);
}

int Print_Queue(Queue *pQueue) {
    //int k = 1;
    printf("\n");
    Proceso *actual;
    actual = pQueue->head;
    while (actual != NULL) {
      printf(" %i -", actual->PID);
      actual = actual->next;
    }
    return FALSE;
}

int Ordered_Enqueue(Queue *pQueue, Proceso *item) { //inserta orndenado dejando el proceso de mayor prioridad en head
    item->next = NULL;
    if (pQueue->size == 0) {
        pQueue->head = item;
        pQueue->tail = item;

    } else {
        int k = 1;
        Proceso *actual;
        actual = pQueue->head;
        if (actual->next == NULL) {
          if (item->prioridad <= actual->prioridad) {
            actual->next = item;
            pQueue ->tail = item;
            k = 0;
          }
          else {
            item->next = actual;
            pQueue->head = item;
            k = 0;
          }
        }
        if (k == 1 && item->prioridad > pQueue->head->prioridad) {
          pQueue->head = item;
          item->next = actual;
          pQueue->size++;
          return TRUE;
        }
        while (k == 1) {
          if (item->prioridad <= actual->next->prioridad) {
            if (actual->next->next == NULL) {
              actual->next->next = item;
              pQueue ->tail = item;
              k = 0;
            }
            else{
              actual = actual->next;
            }
          }
          else{
            item->next = actual->next;
            actual->next = item;
            k = 0;
          }
        }
    }
    pQueue->size++;
    return TRUE;
}

int Enqueue(Queue *pQueue, Proceso *item) {
    item->next = NULL;
    if (pQueue->size == 0) {
        pQueue->head = item;
        pQueue->tail = item;

    } else {
        pQueue->tail->next = item;
        pQueue->tail = item;
    }
    pQueue->size++;
    return TRUE;
}

Proceso * Dequeue(Queue *pQueue) {  //extrae el proceso de mayor prioridad (head)
    Proceso *item;
    if (isEmpty(pQueue))
        return NULL;
    item = pQueue->head;
    pQueue->head = (pQueue->head)->next;
    pQueue->size--;
    return item;
}

int isEmpty(Queue* pQueue) {
    if (pQueue == NULL) {
        return FALSE;
    }
    if (pQueue->size == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}
