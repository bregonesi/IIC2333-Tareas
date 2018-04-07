#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

	//char matriz_procesos[300][255]; //por mientras, despues hay que hacerlo dinamico (no se sabe cuantos procesos son)
	//int **matriz_tiempos = malloc(sizeof(float *) * 300); //lo mismo
	char nombre_proceso[255];
	int tiempo_inicio;
	int cantidad_valores;
	int valor_actual;
	int pid = 0;
	while (fscanf(archivo_procesos, "%s", nombre_proceso) != EOF) {
		printf("PID: %i\n", pid++);
		printf("nombre_proceso: %s\n", nombre_proceso);
		fscanf(archivo_procesos, "%i", &tiempo_inicio);
		printf("tiempo inicio: %i\n", tiempo_inicio);
		fscanf(archivo_procesos, "%i", &cantidad_valores);
		printf("cantidad valores: %i\n", cantidad_valores);

		for(int i = 0; i < cantidad_valores; i++) {
			fscanf(archivo_procesos, "%i", &valor_actual);
			printf("valor actual: %i\n", valor_actual);
		}
	}
	fclose(archivo_procesos);

	printf("quantum: %i, queues: %i\n", quantum, queues);

	return 0;
}
