#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "colas.h"

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
	if (argc == 6) {
		int S = atoi(argv[5]); //guardar S solo si estamos en v2 o v3
		printf("S: %i\n", S);
	}
	if (!(archivo_procesos)) {
		printf("Archivo %s no existe\n", input_file);
		return 1;
	}

	Queue *cola_por_nacer = ConstructQueue(-1);

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

		Ordered_Enqueue(cola_por_nacer, proceso);
	}
	//Print_Queue(cola_por_nacer);
	fclose(archivo_procesos);

	Queue_Queue *colas = ConstructMLFQueue(queues);

	int T = 0;
	while(TRUE) {
		if(T == 200) break;

		Proceso *nacer = Born(*cola_por_nacer, T);
		if(nacer != NULL) {
			Dequeue(cola_por_nacer);
			Enqueue(colas->head, nacer);
			nacer->estado = READY;
			printf("Nace en t=%i\n", nacer->prioridad);
		}

		Ejecutar_proceso(colas, quantum);

		if (argc == 6) {
			// hacer lo del S
		}


		T++;
	}

	Queue_Print_Queue(colas);

	return 0;
}
