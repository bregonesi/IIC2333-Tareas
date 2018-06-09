#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Mazo {
	int** cartas;
	int cantidad_cartas;
} Mazo;

Mazo* crear_mazo();
void print_mazo(Mazo mazo);
int* sacar_carta(Mazo* mazo);
