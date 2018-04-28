#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>

#include <sys/stat.h>
#include <fcntl.h>

#include "cola.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

struct timeval  tv1, tv2, tvtemp;
int m;
int n;
int m_exec = 0;
pid_t *procesos;
Queue* tareas;
Queue* tareas_finalizadas;

void stats_print() {
	printf("\n\n");
	printf("------Outputs------\n");
	int c;
	FILE *file;
	file = fopen("tmp.txt", "r");
	if (file) {
		while ((c = getc(file)) != EOF) {
			printf("%c", c);
	    //putchar(c);
		}
	  fclose(file);
	}
	unlink("tmp.txt");

	printf("\n\n");
	printf("------Estadisticas------\n");
	gettimeofday(&tv2, NULL);
	//double tiempo_paralelo = (double)(end - begin) / CLOCKS_PER_SEC;
	//printf("Procesos ejecutados: %i\n", m-tareas->size); //por ahora esta asi hasta preguntar al ayudante
	printf("Procesos ejecutados: %i\n", m_exec);
	printf("Valor de m: %i\n", m);
	printf("Valor de n: %i\n", n);
	printf("Tiempo real (tiempo paralelo): %fs\n", (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
       (double) (tv2.tv_sec - tv1.tv_sec));

	double t_total = 0;
	NODE* proc = tareas_finalizadas->head;
	while(proc) {
		t_total += proc->t_intento1;
		t_total += proc->t_intento2;
		//printf("t1: %fs\n", proc->t_intento1);
		//printf("t2: %fs\n", proc->t_intento2);
		proc = proc->next;
	}

	printf("Tiempo total (tiempo secuencial): %fs\n", t_total);
}

void stats(int sig) {
		for(int i = 0; i < m; i++) {
			if(procesos[i] && kill(procesos[i], SIGTERM) != -1)  // killeamos
				printf("Prceso %d finished\n", procesos[i]);
			//kill(procesos[i], SIGTERM)
		}
		stats_print();
    //close(0);  // foo is displayed if this line is commented out
    _Exit(0);
}

void child_interruption(int sig) {
	printf("Proceso %d auto-finished\n", getpid());
	_Exit(0);
}

int main(int argc, char *argv[])
{
	gettimeofday(&tv1, NULL);

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

	tareas = ConstructQueue();

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
			params++;

			if(!args) args = malloc(sizeof(char*) * 2);

			if(params > 1) {  // hay que expandir args
				args = realloc(args, sizeof(char*) * (params + 1));
			}

			args[params - 1] = calloc(strlen(line) + 1, sizeof(char));
			strcpy(args[params - 1], line);  // aqui se pasa a una lista para que sea ejecutado por execvp

			if(ch == '\n') {
				//for(int i = 0; i <= params; i++) printf("%s\n", args[i]);
				args[params] = NULL;

				NODE* nodo = malloc(sizeof(NODE));

				nodo->lista_args = args;
				nodo->size = params;
				nodo->intentos = 0;
				nodo->t_intento1 = 0;
				nodo->t_intento2 = 0;

				Enqueue_last(tareas, nodo);

				//execvp(args[0], args);  // asi se ejecuta el comando
			}
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
 	procesos = calloc(MIN(n, m), sizeof(fork()));
	NODE **en_ejecucion = malloc(sizeof(NODE*) * MIN(n, m));
	tareas_finalizadas = ConstructQueue();
	int i = 0;
	int status;  // retorna status del proceso finalizado

	signal(SIGINT, stats);  // por si se hace ctrl + c

	int fdout;
	if((fdout = open("tmp.txt", O_CREAT|O_TRUNC|O_WRONLY, 0644)) < 0) {
		perror("tmp.txt");   /* open failed */
		exit(1);
	}

	while(true) {

		NODE* ejecutar = Dequeue(tareas);

		if(i >= n || !ejecutar) {
			pid_t wpid = wait(&status);  // espera hasta que termine alguno (primero que temrine)
			printf("PARENT: Child's exit code is: %d and pid %d\n", WEXITSTATUS(status), wpid);

			int j = 0;
			for( ; j < MIN(n, m); j++) {
				if(procesos[j] == wpid) {  // buscamos el que termino
					break;
				}
			}

			char* exit_string = calloc(256, sizeof(char));
			sprintf(exit_string, "The exit code of pid %d (arg0 %s) is %d\n", wpid, en_ejecucion[j]->lista_args[0], WEXITSTATUS(status));
			write(fdout, exit_string, 256);
			free(exit_string);

			if(status == 0 || en_ejecucion[j]->intentos >= 2) {  // si finalizo correctamente o no puede volver a intentar
				gettimeofday(&tvtemp, NULL);
				if(en_ejecucion[j]->intentos == 1)  // ahora calculamos el delta que demoro en ejecutarse
					en_ejecucion[j]->t_intento1 = (double) (tvtemp.tv_usec - en_ejecucion[j]->tv1.tv_usec) / 1000000 +
				       (double) (tvtemp.tv_sec - en_ejecucion[j]->tv1.tv_sec);
				else
					en_ejecucion[j]->t_intento2 = (double) (tvtemp.tv_usec - en_ejecucion[j]->tv2.tv_usec) / 1000000 +
				       (double) (tvtemp.tv_sec - en_ejecucion[j]->tv2.tv_sec);


				Enqueue_last(tareas_finalizadas, en_ejecucion[j]);
			} else {
				if(en_ejecucion[j]->intentos <= 1) {
					if (en_ejecucion[j]->intentos == 1) {
						gettimeofday(&tvtemp, NULL);
						en_ejecucion[j]->t_intento1 = (double) (tvtemp.tv_usec - en_ejecucion[j]->tv1.tv_usec) / 1000000 +
					       (double) (tvtemp.tv_sec - en_ejecucion[j]->tv1.tv_sec);
					}
					if(ejecutar) Enqueue_first(tareas, ejecutar); // no ejecutamos el que ibamos a ejecutar
					ejecutar = en_ejecucion[j];
				}
			}

			if(ejecutar) {  // por si aun quedan por ejecutar
				if((procesos[j] = fork()) == 0) {  // child here
					signal(SIGINT, child_interruption);
					printf("Hijo creado con pid %d y arg0 %s\n", getpid(), ejecutar->lista_args[0]);

					dup2(fdout, 1);
					execvp(ejecutar->lista_args[0], ejecutar->lista_args);
					exit(1);  // por si execvp falla
					// child no ejecuta nada mas
				} else {
					m_exec++;
					ejecutar->pid = procesos[j];
					ejecutar->intentos++;

					if(ejecutar->intentos == 1)
						gettimeofday(&ejecutar->tv1, NULL);
					else
						gettimeofday(&ejecutar->tv2, NULL);

					en_ejecucion[j] = ejecutar;
					printf("Creando hijo con pid %d\n", procesos[j]);
				}
			}

		}

		if(i < MIN(n, m) && ejecutar) {
			if(procesos[i] == 0) {
				if((procesos[i] = fork()) == 0) {  // child here
					signal(SIGINT, child_interruption);
					printf("Hijo creado con pid %d y arg0 %s\n", getpid(), ejecutar->lista_args[0]);

					dup2(fdout, 1);
					execvp(ejecutar->lista_args[0], ejecutar->lista_args);
					exit(1);  // por si execvp falla
					// child no ejecuta nada mas
				} else {
					m_exec++;
					ejecutar->pid = procesos[i];
					ejecutar->intentos++;

					if(ejecutar->intentos == 1)
						gettimeofday(&ejecutar->tv1, NULL);
					else
						gettimeofday(&ejecutar->tv2, NULL);

					en_ejecucion[i] = ejecutar;
					printf("Creando hijo con pid %d\n", procesos[i]);
				}
				i++;
			}
		}

		if(tareas_finalizadas->size >= m) break;

	}

	stats_print();

	DestructQueue(tareas);
	free(procesos);
	free(en_ejecucion);
	DestructQueue(tareas_finalizadas);

	return 0;
}
