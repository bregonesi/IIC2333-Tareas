#include <stdlib.h>
#include <stdio.h>
#include "colas.h"

#define TRUE  1
#define FALSE	0

/*
int Ejecutar_proceso2(Queue_Queue *pQueue, int quantum) {
  Queue *cola_actual;
  cola_actual = pQueue->head;
  while (isEmpty(*cola_actual)==TRUE) { //vamos bajando por prioridades hasta encontrar cola con procesos
    cola_actual = cola_actual->next;
    if (cola_actual == NULL) {
      return 1; //no quedan procesos en el sistema
    }
  }

  Proceso *proceso_actual;
  proceso_actual = cola_actual->head;
  if (proceso_actual->quantum_restante == 0) proceso_actual->quantum_restante = quantum; // por si no ha ejecutado nada

  Time_Queue *tiempos;
  tiempos = proceso_actual->linea_de_tiempo;
  Time *tiempo_actual;
  tiempo_actual = tiempos->head;

  int valor_actual = tiempo_actual->valor;

  proceso_actual->quantum_restante--;  // disminuye solo una unidad
  valor_actual--;  // disminuye solo una unidad

  if (proceso_actual->quantum_restante == 0) {  // utilizo todo su quantom, se cambia a una cola con mayor prioridad
    if (valor_actual == 0) {  // justo cuando se le acaba quantum tb se le acaba valor actual
      TimeDequeue(proceso_actual->linea_de_tiempo); //se saca el burst que ya se completo
    }

    Dequeue(cola_actual); //sacamos al proceso de la cola

    if (!TimeisEmpty(proceso_actual->linea_de_tiempo)) { //ver si el proceso no se ha hecho completamente
      Enqueue(cola_actual->next, proceso_actual);   // se deja en cola con menor prioridad
    }
  } else {  // no ha utilizado todo su quantum
    if (valor_actual == 0) { // se acabo ese burst, se deja con quantum que le queda y pasa al final de la lista
      TimeDequeue(proceso_actual->linea_de_tiempo); //se saca el burst que ya se completo
      Dequeue(cola_actual); // sacamos al proceso de la cola

      if (!TimeisEmpty(proceso_actual->linea_de_tiempo)) { //ver si el proceso no se ha hecho completamente
        Enqueue(cola_actual, proceso_actual);   // se deja al final de la misma cola
      }
    }
  }

  return 0;
}
*/

int Aging(Queue_Queue *pQueue){
  Queue *cola_actual;
  Queue *cola_auxiliar;
  Proceso *proceso_actual;
  cola_actual = pQueue->head;
  while (cola_actual->next != NULL) { //vamos bajando por prioridades hasta la cola
    cola_auxiliar = cola_actual->next;
    while (isEmpty(*cola_auxiliar) == FALSE) {
      proceso_actual = Dequeue(cola_auxiliar);
      Enqueue(cola_actual, proceso_actual);
    }
    cola_actual = cola_auxiliar;
  }
  return 0;
}

int Ejecutar_proceso(Queue_Queue *pQueue, int quantum) {
  Queue *cola_actual;
  cola_actual = pQueue->head;
  while (isEmpty(*cola_actual)==TRUE) { //vamos bajando por prioridades hasta encontrar cola con procesos
    cola_actual = cola_actual->next;
    if (cola_actual == NULL) {
      return 1; //no quedan procesos en el sistema
    }
  }

  Proceso *proceso_actual;
  proceso_actual = cola_actual->head;
  if (proceso_actual->quantum_restante == 0) {  //si es primera vez que se ejecuta
    proceso_actual->quantum_restante = quantum;
  }

  Time_Queue *tiempos;
  tiempos = proceso_actual->linea_de_tiempo;
  Time *tiempo_actual;
  tiempo_actual = tiempos->head;
  //int valor_actual = tiempo_actual->valor;

  tiempo_actual->valor -= 1;  //ejecutamos un clock
  proceso_actual->quantum_restante -= 1;  //disminuimos el restante

  if (tiempo_actual->valor == 0) {  //ya termino el burst
    TimeDequeue(proceso_actual->linea_de_tiempo); //se saca el burst que ya se completo
    if (TimeisEmpty(proceso_actual->linea_de_tiempo)) { //ver si el proceso ya se hizo completamente
      Dequeue(cola_actual); //sacamos al proceso de la cola
      // hay que ponerlo en proicesos terminados
    } else {
      proceso_actual = Dequeue(cola_actual);  //se saca de la cola actual
      Enqueue(cola_actual, proceso_actual);   //se deja al final de la cola actual
    }
  }

  if (proceso_actual->quantum_restante == 0 && tiempo_actual->valor > 0) { //ya termino su quantum y tiene que bajar de cola
    if (cola_actual != pQueue->tail) { //si no es la cola de menor prioridad
      proceso_actual = Dequeue(cola_actual);  //se saca de la cola actual
      cola_actual = cola_actual->next;  //cola de menor prioridad
      Enqueue(cola_actual, proceso_actual);   //se deja al final de la cola con menor prioridad que la actual
    }
    else {
      proceso_actual = Dequeue(cola_actual);  //se saca de la cola actual
      Enqueue(cola_actual, proceso_actual);   //se deja al final de la cola actual
    }
  }
  return 0;
}


Queue_Queue *ConstructMLFQueue(int k) { //k corresponde a la cantidad de queues para el algoritmo
    Queue_Queue *gran_cola = malloc(sizeof(Queue_Queue));
    gran_cola->size = 0;
    gran_cola->head = NULL;
    gran_cola->tail = NULL;

    for (int i = 0; i < k; i++) {
      Queue *cola_actual = ConstructQueue(i);
      Queue_Enqueue(gran_cola, cola_actual);
    }
    return gran_cola;
}

Queue *ConstructQueue(int prioridad) {
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    queue->size = 0;
    queue->prioridad = prioridad;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void DestructQueue(Queue *queue) {
    Proceso *pN;
    while (!isEmpty(*queue)) {
        pN = Dequeue(queue);
        free(pN);
    }
    free(queue);
}

int Print_Queue(Queue *pQueue) {
    printf("Print de cola queue\n");
    printf("Cola size: %i\n", pQueue->size);
    printf("Cola prioridad: %i\n", pQueue->prioridad);
    Proceso *actual;
    actual = pQueue->head;
    int i = 0;
    printf("--i | PID\n");
    while (actual != NULL) {
      printf("--%i: %i", i++, actual->PID);
      printf("\n");
      Print_Proceso(actual);
      actual = actual->next;
    }
    return FALSE;
}

int Queue_Print_Queue(Queue_Queue *pQueue) {
    printf("Cola size: %i\n", pQueue->size);
    Queue *actual;
    actual = pQueue->head;
    printf("----Printeando gran cola----\n");
    while (actual != NULL) {
      printf("Cola con prioridad: %i\n", actual->prioridad);
      Print_Queue(actual);
      actual = actual->next;
      printf("\n");
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
          if (item->prioridad >= actual->prioridad) {
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
        if (k == 1 && item->prioridad < pQueue->head->prioridad) {
          pQueue->head = item;
          item->next = actual;
          pQueue->size++;
          return TRUE;
        }
        while (k == 1) {
          if (item->prioridad >= actual->next->prioridad) {
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

int Queue_Enqueue(Queue_Queue *pQueue, Queue *item) {
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

Proceso *Dequeue(Queue *pQueue) {  //extrae el proceso de mayor prioridad (head)
    Proceso *item;
    if (isEmpty(*pQueue))
        return NULL;
    item = pQueue->head;
    pQueue->head = (pQueue->head)->next;
    pQueue->size--;
    return item;
}

int isEmpty(Queue pQueue) {
    /*if (pQueue) {
        return FALSE;
    }*/
    if (pQueue.size == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

Proceso *Born(Queue pQueue, int time) {
  if(isEmpty(pQueue)) return NULL;
  Proceso *proc = pQueue.head;
  if(time < proc->prioridad) return NULL;
  return proc;
}
