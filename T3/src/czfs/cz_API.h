#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

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

/* Funciones de tarea */
int cz_exists(char* filename);
void cz_ls();

/* Funciones de bitmap */
int bitmap_get_free();
bool bitmap_is_free(int pos);  // pos corresponde a una posicion en el disco
int bitmap_set_first();
int bitmap_de_bloque(int bloque);

/* Manejo de numeros */
char* itoa(int value, char* result, int base);
char* fill_binario(char* binario, int cantidad);
int bin_to_dec(char* bin);
