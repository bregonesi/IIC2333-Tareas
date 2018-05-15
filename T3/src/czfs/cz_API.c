#include "cz_API.h"

czFILE* cz_open(char* filename, char mode) {
  czFILE* file = NULL;
  if(mode == 'r') {
    FILE* fp = fopen(ruta_bin, "rb");

    int i = 0;
    while(i < 1024) {
      char valid[1];
      fread(valid, 1, 1, fp);
      char name[11];
      fread(name, 11, 1, fp);
      char indice[4];
      fread(indice, 4, 1, fp);

      if(atoi(valid) && strcmp(name, filename) == 0) {  // falta chequear si el bitmap esta ocupado // si existe
        file = malloc(sizeof(czFILE));
        file->nombre = malloc(sizeof(char) * 11);
        strcpy(file->nombre, name);

        fseek(fp, atoi(indice), SEEK_SET);
        char tamano[4];
        fread(tamano, 4, 1, fp);
        file->tamano = atoi(tamano);
        char creacion[4];
        fread(creacion, 4, 1, fp);
        file->creacion = atoi(creacion);
        char modificacion[4];
        fread(modificacion, 4, 1, fp);
        file->modificacion = atoi(modificacion);
        char direccion_puntero[1];
        fread(direccion_puntero, 1, 1, fp);
        file->direccion_punteros = atoi(direccion_puntero);
        char next_bloque[4];
        fseek(fp, 1007, SEEK_CUR);  // 1007 + 1 = 1008
        fread(next_bloque, 4, 1, fp);
        file->next_bloque = atoi(next_bloque);

        fclose(fp);

        return file;
      }

      i += 16;
    }

    fclose(fp);

  }
  if(mode == 'w') {
    file = malloc(sizeof(czFILE));
    file->nombre = malloc(sizeof(char) * 11);
    strcpy(file->nombre, filename);

    // falta direccion de los demas dependiendo de la direccion disponible en memoria
  }

  return file;
}

int cz_exists(char* filename) {
  FILE* fp = fopen(ruta_bin, "rb");

  int i = 0;
  while(i < 1024) {
    char valid[1];
    fread(valid, 1, 1, fp);
    char name[11];
    fread(name, 11, 1, fp);
    char indice[4];
    fread(indice, 4, 1, fp);

    if(atoi(valid) && strcmp(name, filename) == 0) {  // falta chequear si el bitmap esta ocupado
      fclose(fp);
      return 1;
    }

    i += 16;
  }

  fclose(fp);
  return 0;
}

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
