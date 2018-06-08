#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Net code and decode */
char* codificar(int mensaje_id, char* mensaje);
char** decodificar(char* codificado);
void free_codificacion(char* codificado);
void free_decodificacion(char** decodificado);

/* Manejo de numeros */
char* itoa(int value, char* result, int base);
char* fill_binario(char* binario, int cantidad);
int bin_to_dec(char* bin);
