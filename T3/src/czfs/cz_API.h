#include "cz_bitmap.h"

/* Funciones de tarea */
int cz_exists(char* filename);
void cz_ls();
czFILE* cz_open(char* filename, char mode);
int cz_write(czFILE* file_desc, void* buffer, int nbytes);
void cz_mount(char* diskfileName);
int cz_close(czFILE* file_desc);
int cz_mv(char* orig, char *dest);
int cz_read(czFILE* file_desc, void* buffer, int nbytes);
int cz_cp(char* orig, char* dest);
void cz_free(czFILE* file);
