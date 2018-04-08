#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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
  int prioridad; // timepo de inicio | se utiliza "prioridad" para tener una EDD menos
	int quantum_restante;
	char nombre[255];
	ProcessStatus estado;
  struct Process *next;
	Time_Queue *linea_de_tiempo;

	/* Stats */
	int n_veces_cpu;  // numero de veces que uso cpu
	int n_veces_int;  // numero de veces que fue interrumpido
	int finish_time;  // para calculo de turnaround TimeEnqueue
	int response_time;  // cuando partio siendo atendido
	bool response_time_setted;  // por si response_time = 0
	// waiting time = finish_time - prioridad - sum(linea de tiempo)
	//int tiempo_total_ejecucion;  // sum(linea de tiempo)
	int waiting_time;  // todo el tiempo que no es atendido
};
typedef struct Process Proceso;

Time_Queue *ConstructTimeQueue();
void DestructTimeQueue(Time_Queue *queue);
int TimeEnqueue(Time_Queue *pQueue, int time);
Time *TimeDequeue(Time_Queue *pQueue);
int TimeisEmpty(Time_Queue* pQueue);
int Print_Proceso(Proceso *proc);
int Print_TimeQueue(Time_Queue *pQueue);
