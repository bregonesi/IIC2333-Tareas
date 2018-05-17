#include "cz_funciones.h"


/* Funciones de bitmap */
int bitmap_get_free();
bool bitmap_entry_is_free(int pos);  // pos corresponde a una posicion en el disco
int bitmap_set_first();
int bitmap_de_bloque(int bloque);
