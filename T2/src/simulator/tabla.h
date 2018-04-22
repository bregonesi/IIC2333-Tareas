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
char*** crear_TLB();
