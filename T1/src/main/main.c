#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


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
  int quantum = atoi(argv[3]);
  int queues = atoi(argv[4]);
	FILE *archivo_procesos;
	archivo_procesos = fopen(input_file, "r");	//leyendo archivo de input
	if (argc == 6) {
		int S = atoi(argv[5]); //guardar S solo si estamos en v2 o v3
	}
	if (!(archivo_procesos)){
		return 1;
	}

	char *matriz_procesos = malloc(sizeof(char) * 300); //por mientras, despues hay que hacerlo dinamico (no se sabe cuantos procesos son)
	int **matriz_tiempos = malloc(sizeof(float *) * 300); //lo mismo
	char linea[255];
	char nombre_proceso[255];
	int tiempo_inicio;
	int cantidad_valores;
	int valor_actual;
	int i = 0; //indice del proceso actual a guardar
	while (fscanf(archivo_procesos, "%s", linea) != EOF) {
		strcpy(nombre_proceso, linea);
		matriz_procesos[i] = &nombre_proceso;
		printf("nombre_proceso: %s\n", nombre_proceso);
		fscanf(archivo_procesos, "%s", linea);
		tiempo_inicio = atoi(linea);
		printf("tiempo inicio: %i\n", tiempo_inicio);
		fscanf(archivo_procesos, "%s", linea);
		cantidad_valores = atoi(linea);
		printf("N: %i\n", cantidad_valores);
		matriz_tiempos[i] = malloc(sizeof(int) * (cantidad_valores + 2));
		matriz_tiempos[i][0] = tiempo_inicio;
		matriz_tiempos[i][1] = cantidad_valores;
		for (int k = 0; k < cantidad_valores; k++) { 	//pasamos todos los A_i del procesos
			fscanf(archivo_procesos, "%s", linea);
			valor_actual = atoi(linea);
			matriz_tiempos[i][k+2] = valor_actual;
			printf("A_%i: %i\n", k, valor_actual);
		}
		i += 1;
	}
	fclose(archivo_procesos);

	printf("quantum: %i, queues: %i\n", quantum, queues);

	return 0;
}
