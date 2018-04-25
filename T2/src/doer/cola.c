#include <stdlib.h>
#include <stdio.h>
#include "cola.h"

#define TRUE  1
#define FALSE	0


Queue *ConstructQueue() {
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void DestructQueue(Queue *queue) {
    NODE *pN;
    while (!isEmpty(queue)) {
        pN = Dequeue(queue);
        if(pN) {
          for(int i = 0; i < pN->size; i++) if(pN->lista_args[i]) free(pN->lista_args[i]);
          free(pN->lista_args);
        }
        free(pN);
    }
    free(queue);
}

int Print_Queue(Queue *pQueue) {
    //int k = 1;
    printf("\n");
    NODE *actual;
    actual = pQueue->head;
    while (actual != NULL) {
      printf(" %d -", actual->pid); // arreglar esto para que se vea la lista
      actual = actual->next;
    }
    return FALSE;
}

int Delete_Node(Queue *pQueue, NODE *item) {
    NODE *actual;
    actual = pQueue->head;
    if (actual == NULL) {
      return FALSE;
    }
    if (actual == item) {
      pQueue->head = actual->next;
    }
    while (actual->next != NULL) {
      if (actual->next == item) {
        actual->next = actual->next->next;
        pQueue->size--;
        return TRUE;
      }
      actual = actual->next;
    }
    return FALSE;
}

int Enqueue_first(Queue *pQueue, NODE *item) {
    item->next = NULL;
    if (pQueue->size == 0) {
        pQueue->head = item;
        pQueue->tail = item;

    } else {
        item->next = pQueue->head;
        pQueue->head = item;
    }
    pQueue->size++;
    return TRUE;
}

int Enqueue_last(Queue *pQueue, NODE *item) {
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

NODE* Get_first(Queue *pQueue) {
    NODE *item;
    if (isEmpty(pQueue))  // cola vacia
        return NULL;
    item = pQueue->head;
    return item;
}

NODE* Get_last(Queue *pQueue) {
    NODE *item;
    if (isEmpty(pQueue))  // cola vacia
        return NULL;
    item = pQueue->tail;
    return item;
}

NODE* Dequeue(Queue *pQueue) {
    NODE *item;
    if (isEmpty(pQueue))  // cola vacia
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
