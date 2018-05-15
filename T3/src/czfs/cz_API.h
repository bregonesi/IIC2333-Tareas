#include <stdio.h>
#include <stdlib.h>


char* ruta_bin;

typedef struct cz_FILE
{
	char* nombre;
  int tamano;
  int creacion;
  int modificacion;
  int direccion_punteros;
  int next_bloque;
} czFILE;

void cz_ls();
