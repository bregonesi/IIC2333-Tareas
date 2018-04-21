#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "opti.h"

int bits_1 = 0;
int bits_2 = 0;
int bits_3 = 0;
int bits_4 = 0;
int bits_5 = 0;
double espacio = 0;


void optimo_1() {
  int bits_1 = 20;
  double tamano1 = (pow(2,20) * 1.375);
  printf("----------------------------------------\n");
  printf("BITS NIVEL 1: %i\n", bits_1);
  printf("ESPACIO UTILIZADO: %g MB\n", (tamano1*(pow(10, -6))));
  printf("----------------------------------------\n");
}

void optimo_2(){
  int i_min;
  double min = +INFINITY;
  double espacio;
  double T_actual;
  struct Tuple t;
  for (int i = 1; i < 20; i++) {
    t = T_2(i);
    T_actual = t.espacio_direccion;
    if (T_actual < min) {
      min = T_actual;
      espacio = t.espacio_total;
      i_min = i;
    }
  }
  bits_1 = i_min;
  bits_2 = 20 - i_min;
  printf("----------------------------------------\n");
  printf("BITS NIVEL 1: %i\n", bits_1);
  printf("BITS NIVEL 2: %i\n", bits_2);
  printf("ESPACIO UTILIZADO: %g MB\n", (espacio*(pow(10, -6))));
  printf("----------------------------------------\n");
}

void optimo_3(){
  int i_min;
  int j_min;
  double min = +INFINITY;
  double espacio;
  double T_actual;
  struct Tuple t;
  for (int i = 1; i < 20; i++) {
    for (int j = 1; j < 19; j++) {
      if ((20 - i - j) > 0) {
        t = T_3(i, j);
        T_actual = t.espacio_direccion;
        if (T_actual < min) {
          min = T_actual;
          espacio = t.espacio_total;
          i_min = i;
          j_min = j;
        }
      }
    }
  }
  bits_1 = i_min;
  bits_2 = 20 - i_min - j_min;
  bits_3 = j_min;
  printf("----------------------------------------\n");
  printf("BITS NIVEL 1: %i\n", bits_1);
  printf("BITS NIVEL 2: %i\n", bits_2);
  printf("BITS NIVEL 3: %i\n", bits_3);
  printf("ESPACIO UTILIZADO: %g MB\n", (espacio*(pow(10, -6))));
  printf("----------------------------------------\n");
}

void optimo_4() {
  int i_min;
  int j_min;
  int p_min;
  double min = +INFINITY;
  double T_actual;
  double espacio;
  struct Tuple t;
  for (int i = 1; i < 20; i++) {
    for (int j = 1; j < 19; j++) {
      for (int p = 1; p < 18; p++) {
        if ((20 - i - j > 2) && (j - p > 1)) {
          t = T_4(i, j, p);
          T_actual = t.espacio_direccion;
          if (T_actual < min) {
            min = T_actual;
            espacio = t.espacio_total;
            i_min = i;
            j_min = j;
            p_min = p;
          }
        }
      }
    }
  }
  bits_1 = i_min;
  bits_2 = 20 - i_min - j_min;
  bits_3 = j_min - p_min;
  bits_4 = p_min;
  printf("----------------------------------------\n");
  printf("BITS NIVEL 1: %i\n", bits_1);
  printf("BITS NIVEL 2: %i\n", bits_2);
  printf("BITS NIVEL 3: %i\n", bits_3);
  printf("BITS NIVEL 4: %i\n", bits_4);
  printf("ESPACIO UTILIZADO: %g MB\n", (espacio*(pow(10, -6))));
  printf("----------------------------------------\n");
}

void optimo_5() {
  int i_min;
  int j_min;
  int p_min;
  int q_min;
  double min = +INFINITY;
  double T_actual;
  double espacio;
  struct Tuple t;
  for (int i = 1; i < 20; i++) {
    for (int j = 1; j < 19; j++) {
      for (int p = 1; p < 18; p++) {
        for (int q = 1; q < 17; q++) {
          if ((20 - i - j > 2) && (j - p > 1) && (p - q > 1)) {
            t = T_5(i, j, p , q);
            T_actual = t.espacio_direccion;
            if (T_actual < min) {
              min = T_actual;
              espacio = t.espacio_total;
              i_min = i;
              j_min = j;
              p_min = p;
              q_min = q;
            }
          }
        }
      }
    }
  }
  bits_1 = i_min;
  bits_2 = 20 - i_min - j_min;
  bits_3 = j_min - p_min;
  bits_4 = p_min- q_min;
  bits_5 = q_min;
  printf("----------------------------------------\n");
  printf("BITS NIVEL 1: %i\n", bits_1);
  printf("BITS NIVEL 2: %i\n", bits_2);
  printf("BITS NIVEL 3: %i\n", bits_3);
  printf("BITS NIVEL 4: %i\n", bits_4);
  printf("BITS NIVEL 5: %i\n", bits_5);
  printf("ESPACIO UTILIZADO: %g MB\n", (espacio*(pow(10, -6))));
  printf("----------------------------------------\n");
}

struct Tuple T_2 (int i) {
  double k1 = powf(2, i) * ((20.0 - i) / 8.0);
  double cantidad_t1 = powf(2, i);
  double k2 = powf(2, 20.0 - i) * (11.0 / 8.0); //tamaño de 1 pag en 2nivel
  double espacio_total = (k1 + (k2*cantidad_t1));
  double espacio_direccion = k1 + k2;
  struct Tuple r = { espacio_total, espacio_direccion };
  return r;
}

struct Tuple T_3 (int i, int j) {
  double k1 = powf(2, i) * ((20.0 - i) / 8.0);
  double cantidad_t1 = powf(2, i);
  double k2 = powf(2, 20.0 - i - j) * (j / 8.0);
  double cantidad_t2 = cantidad_t1 * powf(2, 20.0 - i - j);
  double k3 = powf(2, j) * (11.0 / 8.0);
  double espacio_direccion = k1 + k2 + k3;
  double espacio_total = (k1 + (k2*cantidad_t1) + (k3*cantidad_t2));
  struct Tuple r = { espacio_total, espacio_direccion };
  return r;
}

struct Tuple T_4(int i, int j, int p) {
  double k1 = powf(2, i) * ((20.0 - i) / 8.0);
  double cantidad_t1 = powf(2, i);
  double k2 = powf(2, 20.0 - i - j) * (j / 8.0);
  double cantidad_t2 = cantidad_t1 * powf(2, 20.0 - i - j);
  double k3 = powf(2, j - p) * (p / 8.0);
  double cantidad_t3 = cantidad_t2 * powf(2, j-p);
  double k4 = powf(2, p) * (11.0 / 8.0);
  double espacio_direccion = k1 + k2 + k3 + k4;
  double espacio_total = (k1 + (k2*cantidad_t1) + (k3*cantidad_t2) + (k4*cantidad_t3));
  struct Tuple r = { espacio_total, espacio_direccion };
  return r;
}

struct Tuple T_5(int i, int j, int p, int q) {
  double k1 = powf(2, i) * ((20.0 - i) / 8.0);
  double cantidad_t1 = powf(2, i);
  double k2 = powf(2, 20.0 - i - j) * (j / 8.0);
  double cantidad_t2 = cantidad_t1 * powf(2, 20.0 - i - j);
  double k3 = powf(2, j - p) * (p / 8.0);
  double cantidad_t3 = cantidad_t2 * powf(2, j-p);
  double k4 = powf(2, p - q) * (q / 8.0);
  double cantidad_t4 = cantidad_t3 * powf(2, p-q);
  double k5 = powf(2, q) * (11.0 / 8.0);
  double espacio_direccion = k1 + k2 + k3 + k4 + k5;
  double espacio_total = (k1 + (k2*cantidad_t1) + (k3*cantidad_t2) + (k4*cantidad_t3) + (k5*cantidad_t4));
  struct Tuple r = { espacio_total, espacio_direccion };
  return r;
}
