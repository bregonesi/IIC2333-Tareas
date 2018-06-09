#include "cartas.h"

Mazo* crear_mazo() {
  Mazo* nuevo_mazo = malloc(sizeof(Mazo*));
  nuevo_mazo->cartas = malloc(52 * sizeof(int*));
  nuevo_mazo->cantidad_cartas = 52;

  for(int pinta = 1; pinta <= 4; pinta++) {
    for(int carta = 1; carta <= 13; carta++) {
      int* carta_mazo = malloc(2 * sizeof(int));
      carta_mazo[0] = pinta;
      carta_mazo[1] = carta;
      nuevo_mazo->cartas[(pinta - 1) * 13 + (carta - 1)] = carta_mazo;
    }
  }

  return nuevo_mazo;
}

void print_mazo(Mazo mazo) {
  for(int i = 0; i < mazo.cantidad_cartas; i++) {
    printf("[%i, %i]\n", mazo.cartas[i][0], mazo.cartas[i][1]);
  }
}

int* sacar_carta(Mazo* mazo) {
  srand(time(NULL));
  int r = rand() % mazo->cantidad_cartas;

  int* carta = mazo->cartas[r];
  for(int i = r; i < mazo->cantidad_cartas; i++) {
    mazo->cartas[i] = mazo->cartas[i + 1];
  }
  mazo->cantidad_cartas--;

  return carta;
}
