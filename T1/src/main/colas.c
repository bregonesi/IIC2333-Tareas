#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

int Aging(Queue_Queue *pQueue) {
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

bool Proceso_Running(Queue_Queue pQueue) {
  Queue *cola_actual;
  cola_actual = pQueue.head;
  while(cola_actual) { // recorremos todas las colas
    if(!isEmpty(*cola_actual)) {  // si tiene elementos
      Proceso *proc;
      proc = cola_actual->head;

      while(proc != NULL) {
        if(proc->estado == RUNNING) return TRUE;
        proc = proc->next;
      }
    }
    cola_actual = cola_actual->next;
  }
  return FALSE;
}

int Ejecutar_proceso(Queue_Queue *pQueue, Queue *eQueue, int T) { //Scheduler
  Queue *cola_actual;
  cola_actual = pQueue->head;

  Proceso *proceso_actual;
  proceso_actual = NULL;

  if(Proceso_Running(*pQueue)) {  // si hay alguno running tenemos que terminarlo sin interrumpir
    printf("Scheduler (t = %i): ", T);
    printf("ejecutando un quantum para proceso ya en running\n");
    while(cola_actual != NULL) { // recorremos todas las colas
      if(!isEmpty(*cola_actual)) {  // si tiene elementos
        proceso_actual = cola_actual->head;

        while(proceso_actual) {
          if(proceso_actual->estado == RUNNING) {
            break;
          }
          proceso_actual = proceso_actual->next;
        }
      }
      if(proceso_actual && proceso_actual->estado == RUNNING) break;
      cola_actual = cola_actual->next;
    }
  } else {  // no hay nada running, escoger el primero
    while(isEmpty(*cola_actual)) { //vamos bajando por prioridades hasta encontrar cola con procesos
      cola_actual = cola_actual->next;
      if (cola_actual == NULL) {  // se nos acaban las colas
        printf("Scheduler (t = %i): ", T);
        printf("no encuentra proceso para ejecutar\n");
        return 1; //no quedan procesos en el sistema
      }
    }
    proceso_actual = cola_actual->head;  // el head es prioridad
    printf("Scheduler (t = %i): ", T);
    printf("escogiendo un nuevo proceso para ejecutar\n");
  }

  if (proceso_actual->quantum_restante == 0) {  //si es primera vez que se ejecuta
    proceso_actual->quantum_restante = cola_actual->quantum;
  }

  Time_Queue *tiempos;
  tiempos = proceso_actual->linea_de_tiempo;
  Time *tiempo_actual;
  tiempo_actual = tiempos->head;
  //int valor_actual = tiempo_actual->valor;

  tiempo_actual->valor--;  //ejecutamos un clock
  proceso_actual->quantum_restante--;  //disminuimos el restante
  //free(tiempo_actual);

  if(proceso_actual->response_time == 0 && !proceso_actual->response_time_setted) {
    proceso_actual->response_time_setted = TRUE;
    proceso_actual->response_time = T - proceso_actual->prioridad;  // seteamos response time para procesos que no parten en T=0
  }

  if(proceso_actual->estado != RUNNING) {
    proceso_actual->estado = RUNNING;
    proceso_actual->n_veces_cpu++;
    printf("Scheduler (t = %i): ", T);
    printf("ejecutando proceso (proceso cambia a estado running): %s\n", proceso_actual->nombre);
  }

  Time *time_free = NULL;
  if (tiempo_actual->valor == 0) {  //ya termino el burst
    printf("Scheduler (t = %i): ", T);
    time_free = TimeDequeue(proceso_actual->linea_de_tiempo);
    //free(TimeDequeue(proceso_actual->linea_de_tiempo)); //se saca el burst que ya se completo

    if (TimeisEmpty(proceso_actual->linea_de_tiempo)) { //ver si el proceso ya se hizo completamente
      //DestructTimeQueue(proceso_actual->linea_de_tiempo);
      //Dequeue(cola_actual); //sacamos al proceso de la cola
      Remove(cola_actual, proceso_actual); //sacamos al proceso de la cola
      printf("termina proceso (proceso cambia a estado finished): %s\n", proceso_actual->nombre);
      proceso_actual->estado = FINISHED;
      proceso_actual->finish_time = T;
      proceso_actual->waiting_time += T;  // para que se cumpla finish_time - prioridad - sum(linea de tiempo)
      Enqueue(eQueue, proceso_actual);  // lo metemos a la cola de finished
    } else {
      proceso_actual->estado = READY;
      //proceso_actual = Dequeue(cola_actual);  //se saca de la cola actual
      Remove(cola_actual, proceso_actual); //sacamos al proceso de la cola
      if (proceso_actual->quantum_restante == 0) {  // si justo calza de que se le acaba quantum
        printf("proceso termino tarea y ocupo todo su quantum (proceso cambia a estado ready): %s\n", proceso_actual->nombre);
        proceso_actual->n_veces_int++;
        if (cola_actual != pQueue->tail) { //si no es la cola de menor prioridad
          cola_actual = cola_actual->next;  //cola de menor prioridad
        }
        Enqueue(cola_actual, proceso_actual);  // se mete a la siguiente cola
      } else {
        printf("proceso termino tarea pero no ocupo todo su quantum (proceso cambia a estado ready): %s\n", proceso_actual->nombre);
        Enqueue(cola_actual, proceso_actual);   //se deja al final de la cola actual
      }
    }
  }

  if (proceso_actual->quantum_restante == 0 && tiempo_actual->valor > 0) { //ya termino su quantum y tiene que bajar de cola
    printf("Scheduler (t = %i): ", T);
    proceso_actual->estado = READY;
    proceso_actual->n_veces_int++;
    if (cola_actual != pQueue->tail) { //si no es la cola de menor prioridad
      printf("proceso ocupo todo su quantum y es interrumpido (proceso cambia a estado ready): %s\n", proceso_actual->nombre);
      //proceso_actual = Dequeue(cola_actual);  //se saca de la cola actual
      Remove(cola_actual, proceso_actual); //sacamos al proceso de la cola
      cola_actual = cola_actual->next;  //cola de menor prioridad
      Enqueue(cola_actual, proceso_actual);   //se deja al final de la cola con menor prioridad que la actual
    }
    else {
      printf("proceso ocupo todo su quantum y es interrumpido, pero no baja de cola ya que esta en la ultima (proceso cambia a estado ready): %s\n", proceso_actual->nombre);
      //proceso_actual = Dequeue(cola_actual);  //se saca de la cola actual
      Remove(cola_actual, proceso_actual); //sacamos al proceso de la cola
      Enqueue(cola_actual, proceso_actual);   //se deja al final de la cola actual
    }
  }

  if(time_free != NULL) free(time_free);
  
  return 0;
}


Queue_Queue *ConstructMLFQueue(int k, int quantum, int v) { //k corresponde a la cantidad de queues para el algoritmo
    Queue_Queue *gran_cola = malloc(sizeof(Queue_Queue));
    gran_cola->size = 0;
    gran_cola->head = NULL;
    gran_cola->tail = NULL;

    for (int i = 0; i < k; i++) {
      Queue *cola_actual;
      if (v == 1) {
        cola_actual = ConstructQueue(i, quantum * (i + 1));
      } else {
        cola_actual = ConstructQueue(i, quantum);
      }
      Queue_Enqueue(gran_cola, cola_actual);
    }
    return gran_cola;
}

Queue *ConstructQueue(int prioridad, int quantum) {
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    queue->size = 0;
    queue->prioridad = prioridad;
    queue->quantum = quantum;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void DestructQueue(Queue *queue) {
    Proceso *pN;
    while (!isEmpty(*queue)) {
        pN = Dequeue(queue);
        DestructTimeQueue(pN->linea_de_tiempo);
        free(pN);
    }
    free(queue);
}

void DestructQueueQueue(Queue_Queue *queue) {
    Queue *pN;
    while (!Queue_isEmpty(*queue)) {
        pN = Queue_Dequeue(queue);
        DestructQueue(pN);
    }
    free(queue);
}

int Print_Queue(Queue *pQueue) {
    printf("Print de cola queue\n");
    printf("Cola size: %i\n", pQueue->size);
    printf("Cola prioridad: %i\n", pQueue->prioridad);
    printf("Cola quantum: %i\n", pQueue->quantum);
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

Queue *Queue_Dequeue(Queue_Queue *pQueue) {  //extrae el proceso de mayor prioridad (head)
    Queue *item;
    if (Queue_isEmpty(*pQueue))
        return NULL;
    item = pQueue->head;
    pQueue->head = (pQueue->head)->next;
    pQueue->size--;
    return item;
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

Proceso *Remove(Queue *pQueue, Proceso *item) {
    if (isEmpty(*pQueue)) {
        return NULL;
      }

    if(item == pQueue->head) {  // si es el head
      pQueue->head = (pQueue->head)->next;
      pQueue->size--;
      return item;
    }

    // si no es head
    Proceso* elem; // este sera el anterior
    elem = pQueue->head;

    while(elem->next != item) {  // buscamos el anterior
      elem = elem->next;
      if (elem == NULL) return NULL;
    }

    if(item == pQueue->tail) { // si es el tail
      pQueue->tail = elem;
      pQueue->size--;
      return item;
    } else {
      elem->next = item->next;
      pQueue->size--;
      return item;
    }

    return NULL;
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

int Queue_isEmpty(Queue_Queue pQueue) {
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
