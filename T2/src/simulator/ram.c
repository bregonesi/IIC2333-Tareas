#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "funciones.h"
#include "ram.h"

int* crear_ram_tiempos(){
  int* RAM_tiempos;
  RAM_tiempos = malloc(sizeof(int*) * 256);
  for(int i = 0; i < 256; i++) RAM_tiempos[i] = 0;
  return RAM_tiempos;
}

char*** crear_ram() {
  char ***RAM;
  RAM = malloc(sizeof(char***) * 256);
  for(int i = 0; i < 256; i++) {
    RAM[i] = malloc(sizeof(char**) * 2);
    RAM[i][0] = malloc(sizeof(char*) * 20);
    RAM[i][0] = "-";  //frame completo
    RAM[i][1] = malloc(sizeof(char) * 21);  // hay que determinar cuantos chars es el tiempo
    //RAM[i][1] = '\0';  // hay que inicializar
    RAM[i][1] = "-";  //time
  }
  return RAM;
};

int insertar_en_ram(char*** RAM, char* frame, int tiempo, int* RAM_tiempos) {
  int insertado = 0;
  int n_frame = 0;
  for(int i = 0; i < 256; i++) {
    if(strcmp(RAM[i][0], "-") == 0 && insertado == 0) { //ver si esta vacio
      RAM[i][0] = frame;
      RAM_tiempos[i] = tiempo;
      insertado = 1;
      n_frame = i;
    }
  }
  if(insertado == 0) { //todos ocupados
    double min = +INFINITY;
    int valor;
    int i_min;
    for (int i = 0; i < 256; i++) {
      valor = RAM_tiempos[i];
      if (valor < min) {
        min = valor;
        i_min = i;
      }
    }
    //hacer swap out de RAM[i_min] y marcarlo en su PTE con 100 <-- falta marcar en el PTE
    RAM[i_min][0] = frame;
    RAM_tiempos[i_min] = tiempo;
    n_frame = i_min;
  }
  return n_frame;
}
