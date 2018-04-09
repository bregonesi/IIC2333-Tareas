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

int TimeEnqueue(Time_Queue *pQueue, int time) {
    Time *item = malloc(sizeof(Time));
    item->valor = time;
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

Time *TimeDequeue(Time_Queue *pQueue) {  //extrae el proceso de mayor prioridad (head)
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

int Print_Proceso(Proceso *proc) {
    printf("Print de proceso\n");
    printf("PID: %i\n", proc->PID);
    printf("Prioridad (tiempo de inicio): %i\n", proc->prioridad);
    printf("Quantum restante: %i\n", proc->quantum_restante);
    printf("Nombre: %s\n", proc->nombre);
    printf("Estado: %i\n", proc->estado);

    printf("--- Stats ---\n");
    printf("Turnos de CPU: %i\n", proc->n_veces_cpu);
    printf("Bloqueos: %i\n", proc->n_veces_int);
    printf("Turnaround time: %i\n", proc->finish_time - proc->prioridad);
    printf("Response time: %i\n", proc->response_time);
    printf("Waiting time: %i\n", proc->waiting_time);

    Print_TimeQueue(proc->linea_de_tiempo);
    return FALSE;
}

int Print_TimeQueue(Time_Queue *pQueue) {
    printf("Print de time queue\n");
    printf("Size: %i\n", pQueue->size);
    Time *actual;
    actual = pQueue->head;
    int i = 0;
    printf("i | Tiempo\n");
    while (actual != NULL) {
      printf("%i: %i", i++, actual->valor);
      printf("\n");
      actual = actual->next;
    }
    return FALSE;
}
