#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int const start_conection = 1;
int const conection_established = 2;
int const ask_nickname = 3;
int const return_nickname = 4;
int const opponent_found = 5;
int const initial_pot = 6;
int const game_start = 7;
int const start_round = 8;
int const game_end = 22;
// falta rellenar el resto

static char mensajes[7][256] = {
  "",  // 0
  "",  // 1
  "",  // 2
  "Ingrese nickname",  // 3
  "",  // 4
  "Nickname del oponente:",  // 5
  "Tienes un pot de:"  // 5
};
