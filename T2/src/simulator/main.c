#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "opti.h"
#include "tabla.h"
#include "funciones.h"
#include "ram.h"

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

  int tiempo = 0; //contador para el LRU
  double page_fault = 0;
  double tlb_hit = 0;

  struct info_bits info;
  char****** tabla = NULL;
  char** TLB;
  char*** RAM;
  int* RAM_tiempos;
  int *TLB_tiempos;
  int* TLB_frames;
  char** RAM_asociaciones;
  if (n == 1) info = optimo_1();
  else if (n == 2) info = optimo_2();
  else if (n == 3) info = optimo_3();
  else if (n == 4) info = optimo_4();
  else if (n == 5) info = optimo_5();
  tabla = crear_tabla_paginas(info.b1, info.b2, info.b3, info.b4, info.b5, n);
  TLB = crear_TLB();
  TLB_tiempos = crear_tlb_tiempos();
  TLB_frames = crear_tlb_frames();
  RAM = crear_ram();
  RAM_tiempos = crear_ram_tiempos();
  RAM_asociaciones = crear_ram_asociaciones();

  FILE *archivo_input;
  archivo_input = fopen(input_file, "r");	// leyendo archivo de input

  if (!(archivo_input)) {
    printf("Archivo %s no existe.\n", input_file);
    return 1;
  }

  char instruccion[100];
	while (fscanf(archivo_input, "%s", instruccion) != EOF) {
    char* binario = malloc(sizeof(char) * (28 + 1));
    itoa(atoi(instruccion), binario, 2);

    binario = fill_binario(binario, 28);

    //printf("%s: %s\n", instruccion, binario);

    int pag1 = 0;
    int pag2 = 0;
    int pag3 = 0;
    int pag4 = 0;
    int pag5 = 0;

    char* cut_binary;

    cut_binary = cut_string(binario, 0, info.b1);
    pag1 = bin_to_dec(cut_binary);

    if (info.b2 != 0) {
      cut_binary = cut_string(binario, info.b1, info.b1 + info.b2);
      pag2 = bin_to_dec(cut_binary);
    }
    if (info.b3 != 0) {
      cut_binary = cut_string(binario, info.b1 + info.b2, info.b1 + info.b2 + info.b3);
      pag3 = bin_to_dec(cut_binary);
    }
    if (info.b4 != 0) {
      cut_binary = cut_string(binario, info.b1 + info.b2 + info.b3, info.b1 + info.b2 + info.b3 + info.b4);
      pag4 = bin_to_dec(cut_binary);
    }
    if (info.b5 != 0) {
      cut_binary = cut_string(binario, info.b1 + info.b2 + info.b3 + info.b4, info.b1 + info.b2 + info.b3 + info.b4 + info.b5);
      pag5 = bin_to_dec(cut_binary);
    }

    char* pagina = tabla[pag1][pag2][pag3][pag4][pag5];

    char* offset_bin = malloc(sizeof(char) * 9);
    offset_bin = cut_string(binario, info.b1 + info.b2 + info.b3 + info.b4 + info.b5, info.b1 + info.b2 + info.b3 + info.b4 + info.b5 + 8);
    offset_bin = fill_binario(offset_bin, 8);
    int offset = bin_to_dec(offset_bin);
    int direccion_fisica_dec = -1;
    char* frame = malloc(sizeof(char) * 257);
    int n_frame;
    char* n_frame_bin = malloc(sizeof(char) * 9);
    for (int i = 0; i < 257; i++) frame[i] = '\0';
    char* direccion_bin_pag = cut_string(binario, 0, 20);
    int i_tlb = indice_tlb(direccion_bin_pag, TLB);
    if(i_tlb != -1) {
      int n_frame = TLB_frames[i_tlb];
      frame = RAM[n_frame][0];
      printf("asdausdfayusfduyatsfdasd\n");
      //strcpy(frame, RAM[n_frame][0]);
      TLB_tiempos[i_tlb] = tiempo;
      RAM_tiempos[n_frame] = tiempo;
      tlb_hit += 1;
    }
    else {
      if(strcmp(pagina, "-") == 0) {  //pagina vacia
        page_fault += 1;
        //ir a buscar a .bin y dejarlo en RAM, despues asignar el frame a esta pag y dejar seteado los extras
        cut_binary = cut_string(binario, 0, info.b1 + info.b2 + info.b3 + info.b4 + info.b5); //direccion de la pagina
        int frame_start = bin_to_dec(cut_binary);
        frame = leer_bin("data.bin", frame_start);  //leer del .bin y obtener el frame completo
        n_frame = insertar_en_ram(RAM, frame, tiempo, RAM_tiempos); //inserta en RAM con LRU y devuelve el slot que ocupa
        hacer_swap(RAM_asociaciones, n_frame, tabla, info.b1, info.b2, info.b3, info.b4, info.b5); //dejar 100 en pag vinculada anterior
        asociar(RAM_asociaciones, cut_binary, n_frame);
        itoa(n_frame, n_frame_bin, 2);
        n_frame_bin = fill_binario(n_frame_bin, 8); //es 8 porque estamos hablando del frame fisico

        char direccion[20] = "";
        strcat(direccion, n_frame_bin);
        strcat(direccion, offset_bin);
        direccion_fisica_dec = bin_to_dec(direccion);
        char PTE[20] = "";
        strcat(PTE, n_frame_bin);
        strcat(PTE, "000"); //porque esta en RAM
        tabla[pag1][pag2][pag3][pag4][pag5] = PTE;  //ese ultimo tiene que ser largo 3
      }
      else {
        char* frame_pag = cut_string(pagina, 0, 8); //numero del frame de la RAM asociado
        char* bits_extras = cut_string(pagina, 8, 11);
        if (strcmp(bits_extras, "100") == 0) { //significa que esta en disco
          page_fault += 1;
          //ir a buscar a .bin y dejarlo en RAM, despues asignar el frame a esta pag y dejar seteado los extras
          cut_binary = cut_string(binario, 0, info.b1 + info.b2 + info.b3 + info.b4 + info.b5); //direccion de la pagina
          int frame_start = bin_to_dec(cut_binary);
          frame = leer_bin("data.bin", frame_start);  //leer del .bin y obtener el frame completo
          n_frame = insertar_en_ram(RAM, frame, tiempo, RAM_tiempos); //inserta en RAM con LRU y devuelve el slot que ocupa
          hacer_swap(RAM_asociaciones, n_frame, tabla, info.b1, info.b2, info.b3, info.b4, info.b5); //dejar 100 en pag vinculada anterior
          asociar(RAM_asociaciones, cut_binary, n_frame);
          itoa(n_frame, n_frame_bin, 2);
          n_frame_bin = fill_binario(n_frame_bin, 8); //es 8 porque estamos hablando del frame fisico
          char direccion[20] = "";
          strcat(direccion, n_frame_bin);
          strcat(direccion, offset_bin);
          direccion_fisica_dec = bin_to_dec(direccion);
          char PTE[20] = "";
          strcat(PTE, n_frame_bin);
          strcat(PTE, "000"); //porque esta en RAM
          tabla[pag1][pag2][pag3][pag4][pag5] = PTE;  //ese ultimo tiene que ser largo 3
        }
        else {  //esta en RAM, bits_extras = 000
          n_frame = atoi(frame_pag);
          frame = RAM[n_frame][0];
          RAM_tiempos[n_frame] = tiempo;
          itoa(n_frame, n_frame_bin, 2);
          n_frame_bin = fill_binario(n_frame_bin, 8); //es 8 porque estamos hablando del frame fisico
          char direccion[20] = "";
          strcat(direccion, n_frame_bin);
          strcat(direccion, offset_bin);
          direccion_fisica_dec = bin_to_dec(direccion);
        }
      } // end del else not tlb
      insertar_en_tlb(TLB, direccion_bin_pag, tiempo, TLB_tiempos, TLB_frames, n_frame);
    }  // aqui termina el insert
    // printeando actual //
    printf("-%s-\n", instruccion);  //direccion virtual
    printf("DIRECCION FISICA: %i\n", direccion_fisica_dec);
    printf("CONTENIDO: %i\n", frame[offset]);
    tiempo += 1;
  }  // aqui termina el while
  printf("----------------------------------------\n");
  printf("PORCENTAJE_PAGE_FAULTS = %g%\n", (page_fault/tiempo)*100);
  //printf("tlb_hit: %g\n", tlb_hit);
  printf("PORCENTAJE_TLB_HITS = %g%\n", (tlb_hit/tiempo)*100);
  // aca printear la TLB //
  printf("----------------------------------------\n");
  //for (int i = 0; i < 64; i++) {
  //  printf("%s\n", TLB[i]);
  //}
  return 0;
}
