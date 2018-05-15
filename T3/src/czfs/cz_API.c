#include "cz_API.h"

void cz_ls() {
  FILE* fp = fopen(ruta_bin, "rb");

  printf("Haciendo ls de %s\n", ruta_bin);  

  int i = 0;
  while(i < 1024) {
    char valid[1];
    fread(valid, 1, 1, fp);
    char name[11];
    fread(name, 11, 1, fp);
    char indice[4];
    fread(indice, 4, 1, fp);

    if(atoi(valid))
      printf("%s\n", name);

    i += 16;
  }

  fclose(fp);
}
