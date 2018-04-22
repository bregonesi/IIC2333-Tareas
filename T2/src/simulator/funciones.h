#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char* itoa(int value, char* result, int base);
int bin_to_dec(char* bin);
char* fill_binario(char* binario, int cantidad, int cantidad_malloc);
char* cut_string(char* string, int inicio, int final);
char* leer_bin(char *filename, int pos);
