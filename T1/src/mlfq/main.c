#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#include "colas.h"

int T = 0;  // variables globales para que signal lea
Queue *cola_terminados;
Proceso **procesos;
int c_procesos;

void stats_print() {
		printf("\n");
		printf("Procesos terminados: %i\n", cola_terminados->size);
		printf("Tiempo total: %i\n", T);
		printf("\n");

		Proceso *proc;
		int procesos_print = 0;
		for(int i = 0; i < c_procesos; i++) {
			proc = procesos[i];
			if(T > proc->prioridad) {  // por si no ha nacido
				procesos_print++;

				printf("%s:\n", proc->nombre);
		    printf("Turnos de CPU: %i\n", proc->n_veces_cpu);
		    printf("Bloqueos: %i\n", proc->n_veces_int);
				if(proc->estado == FINISHED) {
		    	printf("Turnaround time: %i\n", proc->finish_time - proc->prioridad);
				} else {
		    	printf("Turnaround time: PROCESS NOT FINISHED\n");
				}
		    printf("Response time: %i\n", proc->response_time);
		    printf("Waiting time: %i\n", proc->waiting_time);

				if(proc) printf("\n");
			}
		}
		if(procesos_print == 0) printf("No ha llegado ningun proceso.\n");

		DestructQueue(cola_terminados);
		free(procesos);
}

void stats(int sig) {
		stats_print();
    //close(0);  // foo is displayed if this line is commented out
    _Exit(0);
}

int main(int argc, char *argv[])
{
	/* El programa recibe 4 parametros (por ahora, despues hay que agregar el de la v3) */
	if(argc != 5 && argc != 6)
	{
		printf("Modo de uso: %s <version> <file> <quantum> <queues> [<S>]\n", argv[0]);
		return 1;
	}

	char* version = argv[1];

	if(argc == 5 && (strcmp(version,"v1") != 0))
	{
		printf("Modo de uso incorrecto: con 4 parametros deberia pedir la v1\n");
		return 1;
	}
	if(argc == 6 && ((strcmp(version,"v2") != 0) && ((strcmp(version,"v3") != 0))))
	{
		printf("Modo de uso incorrecto: con 5 parametros deberia pedir la v2 o v3\n");
		return 1;
	}

	char* input_file = argv[2];
  int quantum = atoi(argv[3]);
  int queues = atoi(argv[4]);
	FILE *archivo_procesos;
	archivo_procesos = fopen(input_file, "r");	//leyendo archivo de input
	int S;
	if (argc == 6) {
		S = atoi(argv[5]); //guardar S solo si estamos en v2 o v3
		printf("S: %i\n", S);
	}
	if (!(archivo_procesos)) {
		printf("Archivo %s no existe\n", input_file);
		return 1;
	}

	Queue *cola_por_nacer = ConstructQueue(-1, -1);
	cola_terminados = ConstructQueue(-1, -1);


	//int T = 0;
	signal(SIGINT, stats);  // por si se hace ctrl + c

	char nombre_proceso[255];
	int tiempo_inicio;
	int cantidad_valores;
	int valor_actual;
	int pid = 0;
	while (fscanf(archivo_procesos, "%s", nombre_proceso) != EOF) {
		Proceso *proceso = malloc(sizeof(Proceso));

		proceso->PID = pid++;
		strcpy(proceso->nombre, nombre_proceso);
		fscanf(archivo_procesos, "%i", &tiempo_inicio);
		proceso->prioridad = tiempo_inicio;
		fscanf(archivo_procesos, "%i", &cantidad_valores);
		proceso->quantum_restante = 0;

		Time_Queue *linea_tiempo = ConstructTimeQueue();

		for(int i = 0; i < cantidad_valores; i++) {
			fscanf(archivo_procesos, "%i", &valor_actual);
			TimeEnqueue(linea_tiempo, valor_actual);
		}

		proceso->linea_de_tiempo = linea_tiempo;

		proceso->n_veces_cpu = 0;
		proceso->n_veces_int = 0;
		proceso->finish_time = 0;
		proceso->response_time = 0;
		proceso->response_time_setted = FALSE;
		proceso->waiting_time = 0;

		Ordered_Enqueue(cola_por_nacer, proceso);
	}
	//Print_Queue(cola_por_nacer);
	fclose(archivo_procesos);

	c_procesos = cola_por_nacer->size;
	procesos = malloc(sizeof(Proceso*) * c_procesos);

	Proceso* proc_i = cola_por_nacer->head;
	for(int i=0; i<c_procesos; i++) {
		procesos[i] = proc_i;
		proc_i = proc_i->next;
	}

	Queue_Queue *colas = ConstructMLFQueue(queues, quantum, !strcmp(version,"v3"));
	//Queue *cola_terminados = ConstructQueue(-1, -1);
	int cantidad_procesos = cola_por_nacer->size;
	bool aging_pendiente = FALSE;

	while(TRUE) {
		if(cola_terminados->size >= cantidad_procesos) break;

		Proceso *nacer = Born(*cola_por_nacer, T);
		if(nacer != NULL) {
			Dequeue(cola_por_nacer);
			Enqueue(colas->head, nacer);
			nacer->estado = READY;
			printf("Proceso %s nace en t = %i\n", nacer->nombre, nacer->prioridad);
		} else {
			free(nacer);
		}

		Ejecutar_proceso(colas, cola_terminados, T);

		if (((strcmp(version,"v2") == 0) || (strcmp(version,"v3") == 0)) && T%S == 0) {
			aging_pendiente = TRUE;
			printf("Ocurre aging de periodo S en t = %i. Queda pendiente.\n", T);
		}

		if(aging_pendiente && !Proceso_Running(*colas)) {
			printf("Ocurre aging de pendiente periodo S en t = %i\n", T);
			Aging(colas);
			aging_pendiente = FALSE;
		}

		T++;
		//usleep(100000);  // eliminar esto, es solo para probar ctrl + c
	}

	stats_print();  // si no se hizo ctrl + c display stats

	DestructQueue(cola_por_nacer);
	DestructQueueQueue(colas);

	return 0;
}
