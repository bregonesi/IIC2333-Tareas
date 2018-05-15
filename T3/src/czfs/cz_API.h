#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
