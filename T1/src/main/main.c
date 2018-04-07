#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
	char version[32];
	/* El programa recibe 4 parametros (por ahora, despues hay que agregar el de la v3) */
	if(argc != 5 && argc != 6)
	{
		printf("Modo de uso: %s <version> <file> <quantum> <queues> [<S>]\n", argv[0]);
		return 1;
	}
	strcpy(version, argv[1]);

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
	char *p;
  int quantum;
  long conv = strtol(argv[3], &p, 10);
  quantum = conv; //guardando quantum
  int queues;
  long conv2 = strtol(argv[4], &p, 10);
  queues = conv2; //guardando queues
	FILE *archivo_procesos;
	archivo_procesos = fopen(input_file, "r");	//leyendo archivo de input
	if (argc == 6) { 
		int S;
	  long conv3 = strtol(argv[5], &p, 10);
	  S = conv3;	//guardar S solo si estamos en v2 o v3
	}

	printf("quantum: %i, queues: %i\n", quantum, queues);

	return 0;
}
