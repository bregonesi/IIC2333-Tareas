#include <stdlib.h>
#include <stdio.h>

#define TRUE  1
#define FALSE	0

typedef struct Node_t
{
  char** lista_args;
  int size;
  int id;
  struct Node_t *next;
} NODE;


typedef struct Queue {    //cola para Open
    NODE *head;
    NODE *tail;
    int size;
    int limit;
} Queue;

Queue *ConstructQueue(int limit);
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, NODE *item);
NODE *Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);
NODE * Get_first(Queue *pQueue);
NODE * Get_last(Queue *pQueue);
int Enqueue_last( Queue *pQueue, NODE *item);
int Enqueue_first(Queue *pQueue, NODE *item);
