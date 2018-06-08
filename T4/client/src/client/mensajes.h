#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int const start_conection = 1;
int const conection_established = 2;
int const ask_nickname = 3;
int const return_nickname = 4;
// falta rellenar el resto

static char mensajes[4][256] = {
  "",  // 0
  "",  // 1
  "",  // 2
  "Ingrese nickname"  // 3
};
