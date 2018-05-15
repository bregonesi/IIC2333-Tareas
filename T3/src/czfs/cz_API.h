#include <stdio.h>

char* ruta_bin = NULL;

typedef struct cz_FILE
{
	char* nombre;
  int tamano;
  int creacion;
  int modificacion;
  int direccion_punteros;
  int next_bloque;
} czFILE;
