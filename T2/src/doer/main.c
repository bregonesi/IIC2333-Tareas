#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>

#include "cola.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

struct timeval  tv1, tv2;
clock_t begin;
int m;
int n;

void stats_print() {
		printf("\n\n\n\n");
		printf("------Estadisticas------\n");
		clock_t end = clock();
		gettimeofday(&tv2, NULL);
		double tiempo_paralelo = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("Valor de m: %i\n", m);
		printf("Valor de n: %i\n", n);
		printf("Tiempo medido con clocks: %fs\n", tiempo_paralelo);
		printf("Tiempo medido con gettime: %fs\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));
}

void stats(int sig) {
		stats_print();
    //close(0);  // foo is displayed if this line is commented out
    _Exit(0);
}

int main(int argc, char *argv[])
{
	begin = clock();
	gettimeofday(&tv1, NULL);
	signal(SIGINT, stats);  // por si se hace ctrl + c

  /* El programa recibe 2 parametros */
	if(argc != 3)
	{
		printf("Modo de uso: %s <file> <n>\n", argv[0]);
		return 1;
	}

  char* input_file = argv[1];
	FILE *archivo_tareas;
	archivo_tareas = fopen(input_file, "r");	// leyendo archivo de input

	if (!(archivo_tareas)) {
		printf("Archivo %s no existe.\n", input_file);
		return 1;
	}

	n = atoi(argv[2]);

	Queue* tareas = ConstructQueue();

  /* Lectura de archivo */

	char line[1024];
	char ch = getc(archivo_tareas);
  int index = 0;
	int params = 0;
	bool colon_param = false;
	char **args = NULL;
	// = malloc(sizeof(char*));
	while ( ch != EOF ) {
		if(ch != '\n') {  // vamos guardando el parametro en linea caracter por caracter
			if((!colon_param && !(ch == ' ' || ch == '"')) || (colon_param && ch != '"')) line[index++] = ch;  // si hay espacios no funciona
    }

		if(ch == '"') {  // si hay algun parametro que use comillas
			if(colon_param) colon_param = false;
			else colon_param = true;
		}

		if((ch == ' ' || ch == '\n') && !colon_param && index != 0) {  // guardamos nuevo parametro
      line[index] = '\0';
      index = 0;

			if(!args) args = malloc(sizeof(char*));

			if(params >= 1) {  // hay que expandir args
				args = realloc(args, sizeof(char*) * (params + 1));
			}

			args[params] = malloc(strlen(line) + 1);
			strcpy(args[params], line);  // aqui se pasa a una lista para que sea ejecutado por execvp

			if(ch == '\n') {
				//for(int i = 0; i <= params; i++) printf("%s\n", args[i]);

				NODE* nodo = malloc(sizeof(NODE));

				nodo->lista_args = args;
				nodo->size = params + 1;
				nodo->intentos = 0;

				Enqueue_last(tareas, nodo);

				//execvp(args[0], args);  // asi se ejecuta el comando
			}
			params++;
		}

		if(ch == '\n') { // por si empieza nueva tarea
			params = 0;
			args = NULL;
		}

		ch = getc(archivo_tareas);
  }
	//free(args);  // pq el ultimo args no se guarda

	fclose(archivo_tareas);

	m = tareas->size;
	pid_t *procesos = calloc(MIN(n, m), sizeof(fork()));
	NODE **en_ejecucion = malloc(sizeof(NODE*) * MIN(n, m));
	Queue* tareas_finalizadas = ConstructQueue();
	int i = 0;
	int status;  // retorna status del proceso finalizado
	while(true) {

		NODE* ejecutar = Dequeue(tareas);

		if(i >= n || !ejecutar) {
			pid_t wpid = wait(&status);  // espera hasta que termine alguno (primero que temrine)
			printf("PARENT: Child's exit code is: %d and pid %d\n", WEXITSTATUS(status), wpid);

			int j = 0;
			for( ; j < n; j++) {
				if(procesos[j] == wpid) {  // buscamos el que termino
					break;
				}
			}

			if(status == 0 || en_ejecucion[j]->intentos >= 2) {  // si finalizo correctamente o no puede volver a intentar
				Enqueue_last(tareas_finalizadas, en_ejecucion[j]);
			} else {
				if(en_ejecucion[j]->intentos <= 1) {
					if(ejecutar) Enqueue_first(tareas, ejecutar); // no ejecutamos el que ibamos a ejecutar
					ejecutar = en_ejecucion[j];
				}
			}

			if(ejecutar) {  // por si aun quedan por ejecutar
				if((procesos[j] = fork()) == 0) {  // child here
					printf("Hijo creado con pid %d y arg0 %s\n", getpid(), ejecutar->lista_args[0]);
					execvp(ejecutar->lista_args[0], ejecutar->lista_args);
					exit(1);  // por si execvp falla
					// child no ejecuta nada mas
				} else {
					ejecutar->pid = procesos[j];
					ejecutar->intentos++;
					en_ejecucion[j] = ejecutar;
					printf("Creando hijo con pid %d\n", procesos[j]);
				}
			}

		}

		if(i < n && ejecutar) {
			if(procesos[i] == 0) {
				if((procesos[i] = fork()) == 0) {  // child here
					printf("Hijo creado con pid %d y arg0 %s\n", getpid(), ejecutar->lista_args[0]);
					execvp(ejecutar->lista_args[0], ejecutar->lista_args);
					exit(1);  // por si execvp falla
					// child no ejecuta nada mas
				} else {
					ejecutar->pid = procesos[i];
					ejecutar->intentos++;
					en_ejecucion[i] = ejecutar;
					printf("Creando hijo con pid %d\n", procesos[i]);
				}
				i++;
			}
		}

		if(tareas_finalizadas->size >= m) break;

	}

	DestructQueue(tareas);
	free(procesos);
	free(en_ejecucion);
	DestructQueue(tareas_finalizadas);
	//sleep(2); //probando ctrl c

	stats_print();

	return 0;
}
