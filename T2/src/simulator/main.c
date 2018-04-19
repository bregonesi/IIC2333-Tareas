#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "opti.h"


int main(int argc, char *argv[])
{
  if(argc != 3)
	{
		printf("Modo de uso: %s <n> <file>\n", argv[0]);
		return 1;
	}
  char* input_file = argv[2];
	FILE *archivo_tareas;
	archivo_tareas = fopen(input_file, "r");	// leyendo archivo de input

	if (!(archivo_tareas)) {
		printf("Archivo %s no existe.\n", input_file);
		return 1;
	}
  int n;
	n = atoi(argv[1]);
  if (n!=1 && n!=2 && n!=3 && n!=4 && n!=5) {
    printf("n debe ser un numero entre 1 y 5\n");
    return 1;
  }
  if (n == 1) {
    optimo_1();
  }
  else if (n == 2) {
    optimo_2();
  }
  else if (n == 3) {
    optimo_3();
  }
  else if (n == 4) {
    optimo_4();
  }
  else if (n == 5) {
    optimo_5();
  }
}
