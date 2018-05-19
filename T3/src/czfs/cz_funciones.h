#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

char* ruta_bin;  // ruta del binario

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct cz_FILE {
	int indice_en_directorio;
	int direccion_bloque;
	char* nombre;
  int tamano;
	int tamano_datos;
  time_t creacion;
  time_t modificacion;
  int next_bloque;
	char modo;  // 'r' o 'w' dependiendo del modo de lectura
	bool closed;  // para no poder volver a escribir
} czFILE;

/* Manejo de numeros */
char* itoa(int value, char* result, int base);
char* fill_binario(char* binario, int cantidad);
int bin_to_dec(char* bin);
void* buffer_desde(void* buffer_original, int tamano);
