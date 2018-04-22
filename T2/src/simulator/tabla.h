#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct PTE {
  int direccion;
};

struct Direcciones {  //separadas porque mas adelante hay que diferenciar en algunos atributos
  int direccion;
};

char****** crear_tabla_paginas(int b1, int b2, int b3, int b4, int b5, int n);
int* crear_tlb_tiempos();
char** crear_TLB();
int* crear_tlb_frames();
int indice_tlb(char* direccion_bin_pag, char** TLB);
int insertar_en_tlb(char** TLB, char* frame, int tiempo, int* TLB_tiempos, int* TLB_frames, int direccion_frame);
