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

        file->direccion_directorio = i;
        file->direccion_bloque = atoi(indice);

        file->nombre = malloc(sizeof(char) * 11);
        strcpy(file->nombre, name);

        fseek(fp, atoi(indice), SEEK_SET);  // nos vemos al archivo
        char tamano[4];
        fread(tamano, 4, 1, fp);
        file->tamano = atoi(tamano);
        char creacion[4];
        fread(creacion, 4, 1, fp);
        file->creacion = atoi(creacion);
        char modificacion[4];
        fread(modificacion, 4, 1, fp);
        file->modificacion = atoi(modificacion);
        char next_bloque[4];
        fseek(fp, 1008, SEEK_CUR);
        fread(next_bloque, 4, 1, fp);  // nos saltamos la data
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



/* Funciones de bitmap */
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

char* fill_binario(char* binario, int cantidad) {
	char* final = calloc(cantidad + 1, sizeof(char));  // calloc por que hay q inicializar

	for(int i = 0; i < cantidad - (int)strlen(binario); i++) strcat(final, "0");
	strcat(final, binario);

	return final;
}

int bin_to_dec(char* bin) {
	int dec = 0;
	char c;

	for(int i = 0; i < strlen(bin); i++) {
		c = bin[i];
		if(c == '1') dec = dec * 2 + 1;
		else if(c == '0') dec *= 2;
	}

	return dec;
}

int bitmap_get_free() {
  FILE* fp = fopen(ruta_bin, "rb+");
  char* b = calloc(2, sizeof(char));
  for(int i = 1023; i < 1024 * 8; i++) {
    fseek(fp, i, SEEK_SET);
    unsigned char byte[1];
    int byte_dec;
    fread(byte, 1, 1, fp);
    byte_dec = byte[0];

    //b[0] = k;
    //fwrite(b, 1, 1, fp);

    char byte_bin[8];
    itoa(byte_dec, byte_bin, 2);
    //printf("byte bin: %s\n", byte_bin);

    char* filled_byte_bin;
    filled_byte_bin = fill_binario(byte_bin, 8);

    //printf("filled byte bin: %s\n", filled_byte_bin);

    for(int j = 0; j < 8; j++) {
      if(filled_byte_bin[j] == '0') {
        filled_byte_bin[j] = '1';
        //printf("filled byte bin after: %s\n", filled_byte_bin);
        int x = bin_to_dec(filled_byte_bin);
        //if (x >= 128) {
        //  x -= 256;
        //}
        //printf("int del byte: %i\n", x);
        b[0] = x;
        fseek(fp, i, SEEK_SET);
        fwrite(b, 1, 1, fp);
        free(filled_byte_bin);
        fclose(fp);
        free(b);
        return (i - 1023) * 8 + j + 1023;
      }
    }

    free(filled_byte_bin);
  }
  free(b);
  fclose(fp);
  return 0;
}
