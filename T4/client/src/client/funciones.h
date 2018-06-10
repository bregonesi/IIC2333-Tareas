#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DecMazo {
  char* mensaje_id;
  char* payload_size;
  int cantidad_cartas;
	int** cartas;
} Decodificar_Mazo;

/* Net code and decode */
char* codificar(int mensaje_id, char* mensaje);
char* codificar_cartas(int mensaje_id, int** cartas, int cantidad_cartas);
char** decodificar(char* codificado);
Decodificar_Mazo* decodificar_cartas(char* codificado);
void free_codificacion(char* codificado);
void free_decodificacion(char** decodificado);
void free_decodificacion_cartas(Decodificar_Mazo* decodificado);

/* Manejo de numeros */
char* itoa(int value, char* result, int base);
char* fill_binario(char* binario, int cantidad);
int bin_to_dec(char* bin);
