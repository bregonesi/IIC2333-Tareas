#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ram.h"

char*** crear_ram() {
  char ***TLB;
  TLB = malloc(sizeof(char***) * 64);
  for (int i = 0; i < 256; i++) {
    TLB[i] = malloc(sizeof(char**) * 2);
    TLB[i][0] = "-";  //frame completo
    TLB[i][1] = "-";  //time
  }
  return TLB;
};
