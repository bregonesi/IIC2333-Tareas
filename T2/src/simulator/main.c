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


  int a=54325;
  char buffer[20];
  itoa(a,buffer,2);   // here 2 means binary
  printf("Binary value = %s\n", buffer);


  return 0;
}
