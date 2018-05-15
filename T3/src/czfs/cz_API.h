#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char* ruta_bin;
unsigned int T;  // tiempo del sistema

typedef struct cz_FILE {
	int direccion_directorio;
	int direccion_bloque;
	char* nombre;
  int tamano;
  int creacion;
  int modificacion;
  int next_bloque;
	char modo;  // 'r' o 'w' dependiendo del modo de lectura
} czFILE;

int cz_exists(char* filename);
void cz_ls();


char* itoa(int value, char* result, int base);
char* fill_binario(char* binario, int cantidad);
int bitmap_get_free();
