#include <stdlib.h>
#include <stdio.h>

typedef struct Tiempo
{
	int valor;
	struct Tiempo *next;
} Time;

typedef struct Time_Queue {    //cola para linea de tiempo de cada proceso
    Time *head;
    Time *tail;
    int size;
} Time_Queue;

enum process_status
{
	RUNNING,
	READY,
	FINISHED
};
typedef enum process_status ProcessStatus;

struct Process
{
	int PID;
  int prioridad; // timepo de inicio
	char *nombre;
	ProcessStatus estado;
  struct Process *next;
	Time_Queue linea_de_tiempo;
};
typedef struct Process Proceso;

Time_Queue *ConstructTimeQueue();
void DestructTimeQueue(Time_Queue *queue);
int TimeEnqueue(Time_Queue *pQueue, Time *item);
Time *TimeDequeue(Time_Queue *pQueue);
int TimeisEmpty(Time_Queue* pQueue);
