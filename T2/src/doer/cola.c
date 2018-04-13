#include <stdlib.h>
#include <stdio.h>
#include "cola.h"

#define TRUE  1
#define FALSE	0


Queue *ConstructQueue(int k) {
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    //queue->limit = limit;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void DestructQueue(Queue *queue) {
    NODE *pN;
    while (!isEmpty(queue)) {
        pN = Dequeue(queue);
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
      printf(" %i -", actual->id); // arreglar esto para que se vea la lista
      actual = actual->next;
    }
    return FALSE;
}

int Delete_Node(Queue *pQueue, NODE *item) {
    //int k = 1;
    NODE *actual;
    actual = pQueue->head;
    if (actual == NULL) {
      return FALSE;
    }
    if (actual-> id == item->id) {
      pQueue->head = actual->next;
    }
    while (actual->next != NULL) {
      if (actual->next->id == item->id) {
        actual->next = actual->next->next;
        pQueue->size--;
        return TRUE;
      }
      actual = actual->next;
    }
    return FALSE;
}

int Ordered_Enqueue(Queue *pQueue, NODE *item) {  //por ahora no tiene mucho sentido, lo dejo con id por mientras
    item->next = NULL;
    if (pQueue->size == 0) {
        pQueue->head = item;
        pQueue->tail = item;

    } else {
        int k = 1;
        NODE *actual;
        actual = pQueue->head;
        if (actual->next == NULL) {
          if (item->id >= actual->id) {
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
        if (k == 1 && item->id < pQueue->head->id) {
          pQueue->head = item;
          item->next = actual;
          pQueue->size++;
          return TRUE;
        }
        while (k == 1) {
          if (item->id >= actual->next->id) {
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

NODE * Get_first(Queue *pQueue) {
    NODE *item;
    if (isEmpty(pQueue))  // cola vacia
        return NULL;
    item = pQueue->head;
    return item;
}

NODE * Get_last(Queue *pQueue) {
    NODE *item;
    if (isEmpty(pQueue))  // cola vacia
        return NULL;
    item = pQueue->tail;
    return item;
}

NODE * Dequeue(Queue *pQueue) {
    NODE *item;
    if (isEmpty(pQueue))  // cola vacia
        return NULL;
    item = pQueue->head;
    pQueue->head = (pQueue->head)->next;
    pQueue->size--;
    return item;
}

int isEmpty(Queue* pQueue) {   //lo dejo pasar porque problema tiene solucion siempre
    if (pQueue == NULL) {
        return FALSE;
    }
    if (pQueue->size == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}
