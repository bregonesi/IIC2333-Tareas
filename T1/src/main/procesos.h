#include <stdlib.h>
#include <stdio.h>

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
  int prioridad;
	char nombre;
	ProcessStatus estado;
  struct Process *next;
};
typedef struct Process Proceso;
