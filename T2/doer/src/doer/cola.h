#include <stdlib.h>
#include <stdio.h>

#define TRUE  1
#define FALSE	0

typedef struct Node_t
{
  char** lista_args;
  int size;
  pid_t pid;
  int intentos;
  struct Node_t *next;
} NODE;


typedef struct Queue {    //cola para Open
    NODE *head;
    NODE *tail;
    int size;
    int limit;
} Queue;

Queue *ConstructQueue();
void DestructQueue(Queue *queue);
int Enqueue_first(Queue *pQueue, NODE *item);
NODE* Dequeue(Queue *pQueue);
int isEmpty(Queue* pQueue);
NODE* Get_first(Queue *pQueue);
NODE* Get_last(Queue *pQueue);
int Enqueue_last( Queue *pQueue, NODE *item);
int Enqueue_first(Queue *pQueue, NODE *item);
int Print_Queue(Queue *pQueue);
