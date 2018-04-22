#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "funciones.h"
#include "ram.h"

void asociar(char** RAM_asociaciones, char* cut_binary, int n_frame){
  RAM_asociaciones[n_frame] = cut_binary;
}

void hacer_swap(char** RAM_asociaciones, int n_frame, char****** tabla, int b1, int b2, int b3, int b4, int b5){
  char* binario = RAM_asociaciones[n_frame];
  char* cut_binary;
  int pag1 = 0;
  int pag2 = 0;
  int pag3 = 0;
  int pag4 = 0;
  int pag5 = 0;
  cut_binary = cut_string(binario, 0, b1);
  pag1 = bin_to_dec(cut_binary);
  if (b2 != 0) {
    cut_binary = cut_string(binario, b1, b1 + b2);
    pag2 = bin_to_dec(cut_binary);
  }
  if (b3 != 0) {
    cut_binary = cut_string(binario, b1 + b2, b1 + b2 + b3);
    pag3 = bin_to_dec(cut_binary);
  }
  if (b4 != 0) {
    cut_binary = cut_string(binario, b1 + b2 + b3, b1 + b2 + b3 + b4);
    pag4 = bin_to_dec(cut_binary);
  }
  if (b5 != 0) {
    cut_binary = cut_string(binario, b1 + b2 + b3 + b4, b1 + b2 + b3 + b4 + b5);
    pag5 = bin_to_dec(cut_binary);
  }
  char* pagina = tabla[pag1][pag2][pag3][pag4][pag5];
  if(strcmp(pagina, "-") != 0) {  //entonces ya habia uno asignado
    char* frame = malloc(sizeof(char) * 9);
    frame = cut_string(pagina, 0, 8); //sacamos solo los del frame
    char PTE[12] = "";
    strcat(PTE, frame);
    strcat(PTE, "100"); //le decimos que ahora esta en HDD
    tabla[pag1][pag2][pag3][pag4][pag5] = PTE;
  }
}

char** crear_ram_asociaciones(){
  char** RAM_asociaciones;
  RAM_asociaciones = malloc(sizeof(char*) * 256);
  for(int i = 0; i < 256; i++) RAM_asociaciones[i] = "-";
  return RAM_asociaciones;
}

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
    RAM[i] = malloc(sizeof(char**));
    RAM[i][0] = malloc(sizeof(char*));
    RAM[i][0] = "-";  //frame completo
    // arreglar aqui
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
