#include "cz_bitmap.h"

/* Funciones de tarea */
int cz_exists(char* filename);
void cz_ls();
czFILE* cz_open(char* filename, char mode);
int cz_write(czFILE* file_desc, void* buffer, int nbytes);
void cz_mount(char* diskfileName);
int cz_write_bloque(int direccion_bloque, void* buffer, int tamano_restante_ultimo_bloque);
