#include <stdlib.h>
#include <stdio.h>
#include "procesos.h"

#define TRUE  1
#define FALSE	0

Time_Queue *ConstructTimeQueue() {
    Time_Queue *queue = (Time_Queue*) malloc(sizeof (Time_Queue));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void DestructTimeQueue(Time_Queue *queue) {
    Time *pN;
    while (!TimeisEmpty(queue)) {
        pN = TimeDequeue(queue);
        free(pN);
    }
    free(queue);
}

int TimeEnqueue(Time_Queue *pQueue, Time *item) {
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

Time * TimeDequeue(Time_Queue *pQueue) {  //extrae el proceso de mayor prioridad (head)
    Time *item;
    if (TimeisEmpty(pQueue))
        return NULL;
    item = pQueue->head;
    pQueue->head = (pQueue->head)->next;
    pQueue->size--;
    return item;
}

int TimeisEmpty(Time_Queue* pQueue) {
    if (pQueue == NULL) {
        return FALSE;
    }
    if (pQueue->size == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}
