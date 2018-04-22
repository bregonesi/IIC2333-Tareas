#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
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
  int tiempo = 0; //contador para el LRU
  int page_fault = 0;
  int tlb_hit = 0;
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

  for (linea in txt) {
    a = linea; //tranformado a 28 bits_2
    int pag1 = 0;
    int pag2 = 0;
    int pag3 = 0;
    int pag4 = 0;
    int pag5 = 0;
    char pag1_c[28];
    itoa(atoi(linea[0,info.b1]), pag1_c, 10);
    pag1 = atoi(pag1_c);
    if (info.b2 != 0) {
      char pag2_c[28];
      itoa(atoi(linea[info.b1,info.b1 + info.b2]), pag2_c, 10);
      pag1 = atoi(pag2_c);
    }
    if (info.b3 != 0) {
      char pag3_c[28];
      itoa(atoi(linea[info.b1 + info.b2 ,info.b1 + info.b2 + info.b3]), pag3_c, 10);
      pag1 = atoi(pag3_c);
    }
    if (info.b4 != 0) {
      char pag4_c[28];
      itoa(atoi(linea[info.b1 + info.b2 + info.b3 ,info.b1 + info.b2 + info.b3 + info.b4]), pag4_c, 10);
      pag1 = atoi(pag4_c);
    }
    if (info.b5 != 0) {
      char pag5_c[28];
      itoa(atoi(linea[info.b1 + info.b2 + info.b3 + info.b4 ,info.b1 + info.b2 + info.b3 + info.b4 + info.b5]), pag5_c, 10);
      pag1 = atoi(pag5_c);
    }
    pagina = tabla[pag1][pag2][pag3][pag4][pag5];
    if (pagina == "-") {
      page_fault += 1;
      //ir a buscar a .bin y dejarlo en RAM, despues asignar el frame a esta pag y dejar seteado los extras
      // print fisica y contenido
    }
    else {
      int frame = pagina[0,8];
      int bits_extras = pagina[8,11];
      int ram_o_disco = bits_extras[0]; // 0 = ram, 1 = disco
      if (ram_o_disco == 1) {
        // buscar a disco y dejar en RAM con LRU
        // print fisica y contenido
      }
      else {
        frame = bin_to_int(frame);
        char* arreglo = RAM[frame];
        // print fisica y contenido
      }
    }
  }

  return 0;
}
