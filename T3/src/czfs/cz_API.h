#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* ruta_bin;

typedef struct cz_FILE {
	char* nombre;
  int tamano;
  int creacion;
  int modificacion;
  int direccion_punteros;
  int next_bloque;
	char modo;  // 'r' o 'w' dependiendo del modo de lectura
} czFILE;

int cz_exists(char* filename);
void cz_ls();
