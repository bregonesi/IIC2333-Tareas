#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "opti.h"


void optimo_1() {
  int bits_1 = 20;
  double tamano1 = (pow(2,20) * 1.375);
  printf("----------------------------------------\n");
  printf("BITS NIVEL 1: %i\n", bits_1);
  printf("ESPACIO UTILIZADO: %g MB\n", (tamano1*(pow(10, -6))));
  printf("----------------------------------------\n");
}

void optimo_2(){
  int bits_1;
  int bits_2;
  int i_min;
  double min = +INFINITY;
  double T_actual;
  for (int i = 1; i < 20; i++) {
    T_actual = T_2(i);
    if (T_actual < min) {
      min = T_actual;
      i_min = i;
    }
  }
  bits_1 = i_min;
  bits_2 = 20 - i_min;
  printf("----------------------------------------\n");
  printf("BITS NIVEL 1: %i\n", bits_1);
  printf("BITS NIVEL 2: %i\n", bits_2);
  printf("ESPACIO UTILIZADO: %g B\n", (min));
  printf("----------------------------------------\n");
}

void optimo_3(){
  int bits_1;
  int bits_2;
  int bits_3;
  int i_min;
  int j_min;
  double min = +INFINITY;
  double T_actual;
  for (int i = 1; i < 20; i++) {
    for (int j = 1; j < 19; j++) {
      if ((20 - i - j) > 0) {
        T_actual = T_3(i, j);
        if (T_actual < min) {
          min = T_actual;
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
  printf("ESPACIO UTILIZADO: %g B\n", (min));
  printf("----------------------------------------\n");
}

void optimo_4() {
  int bits_1;
  int bits_2;
  int bits_3;
  int bits_4;
  int i_min;
  int j_min;
  int p_min;
  double min = +INFINITY;
  double T_actual;
  for (int i = 1; i < 20; i++) {
    for (int j = 1; j < 19; j++) {
      for (int p = 1; p < 18; p++) {
        if ((20 - i - j > 2) && (j - p > 1)) {
          T_actual = T_4(i, j, p);
          if (T_actual < min) {
            min = T_actual;
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
  printf("ESPACIO UTILIZADO: %g B\n", (min));
  printf("----------------------------------------\n");
}

void optimo_5() {
  int bits_1;
  int bits_2;
  int bits_3;
  int bits_4;
  int bits_5;
  int i_min;
  int j_min;
  int p_min;
  int q_min;
  double min = +INFINITY;
  double T_actual;
  for (int i = 1; i < 20; i++) {
    for (int j = 1; j < 19; j++) {
      for (int p = 1; p < 18; p++) {
        for (int q = 1; q < 17; q++) {
          if ((20 - i - j > 2) && (j - p > 1) && (p - q > 1)) {
            T_actual = T_5(i, j, p, q);
            if (T_actual < min) {
              min = T_actual;
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
  printf("ESPACIO UTILIZADO: %g B\n", (min));
  printf("----------------------------------------\n");
}

double T_2(int i) {
  double k1 = powf(2, i) * ((20.0 - i) / 8.0);
  double k2 = powf(2, 20.0 - i) * (11.0 / 8.0);
  return (k1 + k2);
}

double T_3(int i, int j) {
  double k1 = powf(2, i) * ((20.0 - i) / 8.0);
  double k2 = powf(2, 20.0 - i - j) * (j / 8.0);
  double k3 = powf(2, j) * (11.0 / 8.0);
  return (k1 + k2 + k3);
}

double T_4(int i, int j, int p) {
  double k1 = powf(2, i) * ((20.0 - i) / 8.0);
  double k2 = powf(2, 20.0 - i - j) * (j / 8.0);
  double k3 = powf(2, j - p) * (p / 8.0);
  double k4 = powf(2, p) * (11.0 / 8.0);
  return (k1 + k2 + k3 + k4);
}

double T_5(int i, int j, int p, int q) {
  double k1 = powf(2, i) * ((20.0 - i) / 8.0);
  double k2 = powf(2, 20.0 - i - j) * (j / 8.0);
  double k3 = powf(2, j - p) * (p / 8.0);
  double k4 = powf(2, p - q) * (q / 8.0);
  double k5 = powf(2, q) * (11.0 / 8.0);
  return (k1 + k2 + k3 + k4 + k5);
}
