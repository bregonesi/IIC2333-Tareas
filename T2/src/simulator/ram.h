#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char*** crear_ram();
int insertar_en_ram(char*** RAM, char* frame, int tiempo, int* RAM_tiempos);
int* crear_ram_tiempos();
char** crear_ram_asociaciones();
void hacer_swap(char** RAM_asociaciones, int n_frame, char****** tabla, int b1, int b2, int b3, int b4, int b5);
void asociar(char** RAM_asociaciones, char* cut_binary, int n_frame);
