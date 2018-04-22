#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "opti.h"
#include "tabla.h"
#include "funciones.h"

int main(int argc, char *argv[])
{
  if(argc != 3)
	{
		printf("Modo de uso: %s <n> <file>\n", argv[0]);
		return 1;
	}

  char* input_file = argv[2];

  int n;
	n = atoi(argv[1]);
  if (n!=1 && n!=2 && n!=3 && n!=4 && n!=5) {
    printf("n debe ser un numero entre 1 y 5\n");
    return 1;
  }

  struct info_bits info;
  char****** tabla = NULL;
  char*** TLB;
  if (n == 1) info = optimo_1();
  else if (n == 2) info = optimo_2();
  else if (n == 3) info = optimo_3();
  else if (n == 4) info = optimo_4();
  else if (n == 5) info = optimo_5();
  tabla = crear_tabla_paginas(info.b1, info.b2, info.b3, info.b4, info.b5, n);
  TLB = crear_TLB();

  FILE *archivo_input;
  archivo_input = fopen(input_file, "r");	// leyendo archivo de input

  if (!(archivo_input)) {
    printf("Archivo %s no existe.\n", input_file);
    return 1;
  }

  char instruccion[10];
	while (fscanf(archivo_input, "%s", instruccion) != EOF) {
    char* binario = malloc(sizeof(char) * 28);
    itoa(atoi(instruccion), binario, 2);

    binario = fill_binario(binario, 28);
    printf("%s: %s\n", instruccion, binario);
  }

  return 0;
}
