#include "cz_API.h"

/* Funciones de tarea */
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

      if(atoi(valid) && strcmp(name, filename) == 0 && !bitmap_entry_is_free(atoi(indice))) { // si existe
        file = malloc(sizeof(czFILE));

        file->direccion_directorio = i;
        file->direccion_bloque = atoi(indice) * 1024;

        file->nombre = malloc(sizeof(char) * 11);
        strcpy(file->nombre, name);

        fseek(fp, atoi(indice), SEEK_SET);  // nos vemos al archivo
        char tamano[4];
        fread(tamano, 4, 1, fp);
        file->tamano = atoi(tamano);
        //file->tamano_datos = 0;
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
        file->modo = 'r';
        file->closed = true;

        return file;
      }

      i += 16;
    }
    fclose(fp);
  }

  if(mode == 'w') {
    //ver si existe, y abrirlo en modo w y retornar NULL
    if (cz_exists(filename))
      return NULL;

    FILE* fp = fopen(ruta_bin, "rb");

    int i = 0;
    while(i < 1024) { //iteramos en el dir
      char valid[1];
      fread(valid, 1, 1, fp);

      if(!atoi(valid)) { // si hay espacio libre en esta parte del dir
        fseek(fp, i, SEEK_SET);  // devolvemos para escribir en el directorio

        fwrite("1", 1, 1, fp);  // ahora es valid
        fwrite(filename, 11, 1, fp);  // guardamos el name

        char indice[4]; //numero del bloque donde se encuentra
        int n_bloque = bitmap_set_first() - 1024; // setea en bitmap el bloque a usar y se guarda la posicion en disco asignada
        itoa(n_bloque, indice, 10);
        fwrite(indice, 4, 1, fp);  // guardamos el indice

        /* Nos metemos al bloque del archivo */
        fseek(fp, n_bloque * 1024, SEEK_SET);

        char tamano[4];
        itoa(12, tamano, 10);  // 12 bytes de metadata
        fwrite(tamano, 4, 1, fp);

        char creacion[4];
        itoa(T, creacion, 10);  // T es nuestra varaible global
        fwrite(creacion, 4, 1, fp);

        char modificacion[4];
        itoa(T, modificacion, 10);  // T es nuestra varaible global
        fwrite(modificacion, 4, 1, fp);

        file = malloc(sizeof(czFILE));
        file->direccion_directorio = i;
        file->direccion_bloque = atoi(indice) * 1024;
        file->nombre = malloc(sizeof(char) * 11);
        strcpy(file->nombre, filename);
        file->modo = 'w';
        file->closed = false;
        file->tamano = 12; //solamente el metadata
        file->tamano_datos = 0; // 0 punteros escritos
        file->creacion = T;
        file->modificacion = T;

        printf("archivo %s creado en bloque %i\n", filename, n_bloque);

        fclose(fp);

        return file;
      }
      i += 16;
    }

    fclose(fp);
  }

  return file; //directorio ya esta lleno, no se como poner otra cosa que no sea NULL para decir esto
}

int cz_exists(char* filename) {
  FILE* fp = fopen(ruta_bin, "rb");

  int i = 0;
  while(i < 1024) {  // 1024 son de directorio
    char valid[1];
    fread(valid, 1, 1, fp);
    char name[11];
    fread(name, 11, 1, fp);
    char indice[4];
    fread(indice, 4, 1, fp);

    if(atoi(valid) && strcmp(name, filename) == 0 && !bitmap_entry_is_free(atoi(indice))) {
      fclose(fp);

      return 1;
    }

    i += 16; //avanza al siguiente bloque
  }
  fclose(fp);

  return 0;
}

int cz_write(czFILE* file_desc, void* buffer, int nbytes) {
  if(file_desc->modo == 'w' && !file_desc->closed) {
    int bytes_escribir = MIN((252 + 256) * 1024 - file_desc->tamano_datos, nbytes);  // maximo que puede escribir

    int tamano_restante_ultimo_bloque = file_desc->tamano_datos % 1024;
    int cantidad_bloques_nuevos = (bytes_escribir - tamano_restante_ultimo_bloque)/1024;

    file_desc->tamano += bytes_escribir + 4 * cantidad_bloques_nuevos;
    file_desc->modificacion = T;  // T es nuestra variable global

    FILE* fp = fopen(ruta_bin, "rb+");

    fseek(fp, file_desc->direccion_directorio + 1, SEEK_SET);  // nos vamos al file name
    char tamano[4];
    itoa(file_desc->tamano, tamano, 10);
    fwrite(tamano, 4, 1, fp);

    //fseek(fp, 4, SEEK_CUR);  // nos saltamos la creacion
    char modificacion[4];
    itoa(file_desc->modificacion, modificacion, 10);  // T es nuestra varaible global
    fwrite(modificacion, 4, 1, fp);

    printf("%s\n", buffer);

    fclose(fp);
  }
  return -1;
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

    if(atoi(valid) && !bitmap_entry_is_free(atoi(indice)))
      printf("%s\n", name);

    i += 16;
  }

  fclose(fp);
}

void cz_mount(char* diskfileName) {
  ruta_bin = diskfileName;
}


/* Funciones de bitmap */
int bitmap_get_free() {
  FILE* fp = fopen(ruta_bin, "rb+");
  char* b = calloc(2, sizeof(char));
  for(int i = 1024; i < 1024 * 8; i++) {
    fseek(fp, i, SEEK_SET);
    unsigned char byte[1];
    int byte_dec;
    fread(byte, 1, 1, fp);
    byte_dec = byte[0];

    char byte_bin[8];
    itoa(byte_dec, byte_bin, 2);

    char* filled_byte_bin;
    filled_byte_bin = fill_binario(byte_bin, 8);
    //printf("%s\n", filled_byte_bin);

    for(int j = 0; j < 8; j++) {
      if(filled_byte_bin[j] == '0') {
        free(filled_byte_bin);
        fclose(fp);
        free(b);

        return (i - 1024) * 8 + j + 1024;
      }
    }
    free(filled_byte_bin);
  }
  free(b);
  fclose(fp);

  return 0;
}

int bitmap_set_first() {
  FILE* fp = fopen(ruta_bin, "rb+");
  char* b = calloc(2, sizeof(char));

  for(int i = 1024; i < 1024 * 8; i++) {
    fseek(fp, i, SEEK_SET);

    unsigned char byte[1];
    int byte_dec;
    fread(byte, 1, 1, fp);
    byte_dec = byte[0];

    char byte_bin[8];
    itoa(byte_dec, byte_bin, 2);

    char* filled_byte_bin;
    filled_byte_bin = fill_binario(byte_bin, 8);

    for(int j = 0; j < 8; j++) {
      if(filled_byte_bin[j] == '0') {
        filled_byte_bin[j] = '1';

        int x = bin_to_dec(filled_byte_bin);
        b[0] = x;

        fseek(fp, i, SEEK_SET);
        fwrite(b, 1, 1, fp);

        free(filled_byte_bin);
        fclose(fp);
        free(b);

        return ((i - 1024) * 8) + j + 1024; //la posicion fisica exacta del bit
      }
    }
    free(filled_byte_bin);
  }
  free(b);
  fclose(fp);

  return 0;
}

bool bitmap_entry_is_free(int pos) {  // pos corresponde a una posicion en el disco
  if(pos >= 1024 && pos < 1024*8) {
    int offset = pos % 1024;
    int bloque = pos - offset;
    //printf("bloque %i offset %i\n", bloque, offset);

    FILE* fp = fopen(ruta_bin, "rb+");
    fseek(fp, bloque, SEEK_SET);
    unsigned char byte[1];
    int byte_dec;
    fread(byte, 1, 1, fp);
    byte_dec = byte[0];
    fclose(fp);

    char byte_bin[8];
    itoa(byte_dec, byte_bin, 2);

    char* filled_byte_bin;
    filled_byte_bin = fill_binario(byte_bin, 8);
    //printf("%s\n", filled_byte_bin);

    char val = filled_byte_bin[offset];
    free(filled_byte_bin);

    if(val == '1')
      return true;
    else
      return false;
  }
  return false;
}

int bitmap_de_bloque(int bloque) {
  int bloque_inicio = bloque - bloque%1024;
  int n_bloque = bloque_inicio/1024;

  return 1024 + n_bloque;
}

/* Manejo de numeros */
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
