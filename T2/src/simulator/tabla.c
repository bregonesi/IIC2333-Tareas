#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tabla.h"

char*** crear_TLB() {
  char ***TLB;
  TLB = malloc(sizeof(char***) * 64);
  for (int i = 0; i < 64; i++) {
    TLB[i] = malloc(sizeof(char**) * 2);
    TLB[i][0] = "-";  //frame
    TLB[i][1] = "-";  //time
  }
  return TLB;
};

char****** crear_tabla_paginas(int b1,int b2,int b3,int b4,int b5,int n) {
  char ******tabla = NULL;
  if (n == 1) {
    tabla = malloc(sizeof(char*****) * (int) pow(2, 20));
    for (int i = 0; i < (int) pow(2,20); i++) {
      tabla[i] = malloc(sizeof(char****));
      tabla[i][0] = malloc(sizeof(char***));
      tabla[i][0][0] = malloc(sizeof(char**));
      tabla[i][0][0][0] = malloc(sizeof(char*));
      tabla[i][0][0][0][0] = "-";
    }
  }
  else if (n == 2) {
    tabla = malloc(sizeof(char*****) * (int) pow(2, b1));
    for (int i = 0; i < (int) pow(2, b1); i++) {
      tabla[i] = malloc(sizeof(char****) * (int) pow(2, b2));
      for (int i2 = 0; i2 < (int) pow(2, b2); i2++) {
        tabla[i][i2] = malloc(sizeof(char***));
        tabla[i][i2][0] = malloc(sizeof(char**));
        tabla[i][i2][0][0] = malloc(sizeof(char*));
        tabla[i][i2][0][0][0] = "-";
      }
    }
  }
  else if (n == 3) {
    tabla = malloc(sizeof(char*****) * (int) pow(2, b1));
    for (int i = 0; i < (int) pow(2, b1); i++) {
      tabla[i] = malloc(sizeof(char****) * (int) pow(2, b2));
      for (int i2 = 0; i2 < (int) pow(2, b2); i2++) {
        tabla[i][i2] = malloc(sizeof(char***) * (int) pow(2, b3));
        for (int i3 = 0; i3 < (int) pow(2, b3); i3++) {
          tabla[i][i2][i3] = malloc(sizeof(char**));
          tabla[i][i2][i3][0] = malloc(sizeof(char*));
          tabla[i][i2][i3][0][0] = "-";
        }
      }
    }
  }
  else if (n == 4) {
    tabla = malloc(sizeof(char*****) * (int) pow(2, b1));
    for (int i = 0; i < (int) pow(2, b1); i++) {
      tabla[i] = malloc(sizeof(char****) * (int) pow(2, b2));
      for (int i2 = 0; i2 < (int) pow(2, b2); i2++) {
        tabla[i][i2] = malloc(sizeof(char***) * (int) pow(2, b3));
        for (int i3 = 0; i3 < (int) pow(2, b3); i3++) {
          tabla[i][i2][i3] = malloc(sizeof(char**) * (int) pow(2, b4));
          for (int i4 = 0; i4 < (int) pow(2, b4); i4++) {
            tabla[i][i2][i3][i4] = malloc(sizeof(char*));
            tabla[i][i2][i3][i4][0] = "-";
          }
        }
      }
    }
  }
  else if (n == 5) {
    tabla = malloc(sizeof(char*****) * (int) pow(2, b1));
    for (int i = 0; i < (int) pow(2, b1); i++) {
      tabla[i] = malloc(sizeof(char****) * (int) pow(2, b2));
      for (int i2 = 0; i2 < (int) pow(2, b2); i2++) {
        tabla[i][i2] = malloc(sizeof(char***) * (int) pow(2, b3));
        for (int i3 = 0; i3 < (int) pow(2, b3); i3++) {
          tabla[i][i2][i3] = malloc(sizeof(char**) * (int) pow(2, b4));
          for (int i4 = 0; i4 < (int) pow(2, b4); i4++) {
            tabla[i][i2][i3][i4] = malloc(sizeof(char*) * (int) pow(2, b5));
            for (int i5 = 0; i5 < (int) pow(2, b5); i5++) {
              tabla[i][i2][i3][i4][i5] = "-";
            }
          }
        }
      }
    }
  }
  return tabla;
};
