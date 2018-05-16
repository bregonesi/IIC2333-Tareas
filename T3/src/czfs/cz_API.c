#include "cz_API.h"

/* Funciones de tarea */
czFILE* cz_open(char* filename, char mode) {
  czFILE* file = NULL;
  if(mode == 'r') {
    FILE* fp = fopen(ruta_bin, "rb");

    int i = 0;
    while(i < 1024) {
      char* valid = calloc(2, sizeof(char));
      fread(valid, 1, 1, fp);
      char* name = calloc(12, sizeof(char));
      fread(name, 11, 1, fp);
      char* indice = calloc(5, sizeof(char));
      fread(indice, 4, 1, fp);

      if(valid[0] == 1 && strcmp(name, filename) == 0 && !bitmap_entry_is_free(atoi(indice))) { // si existe
        file = malloc(sizeof(czFILE));

        file->indice_en_directorio = i;
        file->direccion_bloque = atoi(indice) * 1024;

        file->nombre = name;

        fseek(fp, atoi(indice), SEEK_SET);  // nos vemos al archivo
        char* tamano = calloc(5, sizeof(char));
        fread(tamano, 4, 1, fp);
        file->tamano = atoi(tamano);
        free(tamano);
        char* creacion = calloc(5, sizeof(char));
        fread(creacion, 4, 1, fp);
        file->creacion = atoi(creacion);
        free(creacion);
        char* modificacion = calloc(5, sizeof(char));
        fread(modificacion, 4, 1, fp);
        file->modificacion = atoi(modificacion);
        free(modificacion);
        char* next_bloque = calloc(5, sizeof(char));
        fseek(fp, 1008, SEEK_CUR);
        fread(next_bloque, 4, 1, fp);  // nos saltamos la data
        file->next_bloque = atoi(next_bloque);
        free(next_bloque);
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

    FILE* fp = fopen(ruta_bin, "rb+");

    int i = 0;
    while(i < 1024) { //iteramos en el dir
      char valid[1];
      fseek(fp, i, SEEK_SET);
      fread(valid, 1, 1, fp);
      if(valid[0] == 0) { // si hay espacio libre en esta parte del dir
        fseek(fp, i, SEEK_SET);  // devolvemos para escribir en el directorio

        char* valid_ch = calloc(2, sizeof(char));
        valid_ch[0] = 1;
        fwrite(valid_ch, 1, 1, fp);  // ahora es valid
        free(valid_ch);

        char* filename_11 = calloc(12, sizeof(char));
        strcpy(filename_11, filename);
        fwrite(filename_11, 11, 1, fp);  // guardamos el name
        free(filename_11);

        char* indice = calloc(5, sizeof(char)); //numero del bloque donde se encuentra
        int n_bloque = bitmap_set_first() - 1024; // setea en bitmap el bloque a usar y se guarda la posicion en disco asignada
        itoa(n_bloque, indice, 10);
        fwrite(indice, 4, 1, fp);  // guardamos el indice

        /* Nos metemos al bloque del archivo */
        fseek(fp, n_bloque * 1024, SEEK_SET);

        char* tamano = calloc(5, sizeof(char));
        itoa(12, tamano, 10);  // 12 bytes de metadata
        fwrite(tamano, 4, 1, fp);

        char* creacion = calloc(5, sizeof(char));
        itoa(T, creacion, 10);  // T es nuestra varaible global
        fwrite(creacion, 4, 1, fp);

        char* modificacion = calloc(5, sizeof(char));
        itoa(T, modificacion, 10);  // T es nuestra varaible global
        fwrite(modificacion, 4, 1, fp);

        file = malloc(sizeof(czFILE));
        file->indice_en_directorio = i;
        file->direccion_bloque = atoi(indice) * 1024;
        file->nombre = malloc(sizeof(char) * 11);
        strcpy(file->nombre, filename);
        file->modo = 'w';
        file->closed = false;
        file->tamano = 12; //solamente el metadata
        file->tamano_datos = 0; // 0 punteros escritos
        file->creacion = T;
        file->modificacion = T;

        free(indice);
        free(creacion);
        free(modificacion);
        free(tamano);

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
    printf("indice: %i\n", atoi(indice));
    if(valid[0] == 1 && strcmp(name, filename) == 0 && !bitmap_entry_is_free(atoi(indice))) {
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
    fseek(fp, 0, SEEK_SET);

    //fseek(fp, file_desc->direccion_directorio + 1, SEEK_SET);  // nos vamos al file name
    char* tamano = calloc(5, sizeof(char));
    itoa(file_desc->tamano, tamano, 10);
    printf("%s\n", tamano);
    fwrite(tamano, 4, 1, fp);
    free(tamano);

    fseek(fp, 4, SEEK_CUR);  // nos saltamos la creacion
    char* modificacion = calloc(5, sizeof(char));
    itoa(file_desc->modificacion, modificacion, 10);  // T es nuestra varaible global
    fwrite(modificacion, 4, 1, fp);
    free(modificacion);

    printf("%s\n", buffer);
    int n_bloque = (file_desc->tamano_datos - tamano_restante_ultimo_bloque)/1024;
    int direccion_bloque;
    int bytes_escritos;
    int sum_bytes_escritos = 0;
    while(bytes_escribir >= 0) {  // en realidad != 0
      direccion_bloque = file_desc->direccion_bloque + 12 + n_bloque * 4;
      if(n_bloque > 251)  // del 0 al 251 va en el espacio de 1008 bytes, del 252 en adelante van en direccionamiento indirecto
        direccion_bloque = file_desc->next_bloque + (n_bloque - 251) * 4;  // le sumo los 1008 + el inicio del otro bloque

      if(tamano_restante_ultimo_bloque >= 0) {  // rellenamos ultimo bloque
        void* buffer_escribir = buffer_desde(buffer, bytes_escritos);
        bytes_escritos = cz_write_bloque(direccion_bloque, buffer_escribir, tamano_restante_ultimo_bloque);
      } else {
        int bytes_escribir_bloque_n = MIN(1024, bytes_escribir);
        bytes_escritos = cz_write_bloque(direccion_bloque, buffer, bytes_escribir_bloque_n);
      }
      sum_bytes_escritos += bytes_escritos;
      bytes_escribir -= bytes_escritos;
      file_desc->tamano_datos += bytes_escritos;
      tamano_restante_ultimo_bloque -= bytes_escritos;
      if(tamano_restante_ultimo_bloque == 0) {
        n_bloque++;
        tamano_restante_ultimo_bloque = 1024;
      }
    }

    fclose(fp);

    return sum_bytes_escritos;  // cambiar a retornar bytes escritos
  }
  return -1;
}

void cz_ls() {
  FILE* fp = fopen(ruta_bin, "rb");

  int i = 0;
  while(i < 1024) {
    char* valid = calloc(2, sizeof(char));
    fread(valid, 1, 1, fp);
    char* name = calloc(12, sizeof(char));
    fread(name, 11, 1, fp);
    char* indice = calloc(6, sizeof(char));
    fread(indice, 4, 1, fp);

    if(valid[0] == 1 && !bitmap_entry_is_free(atoi(indice)))
      printf("%s\n", name);

    free(valid);
    free(name);
    free(indice);

    i += 16;
  }

  fclose(fp);
}

void cz_mount(char* diskfileName) {
  ruta_bin = diskfileName;
}

int cz_write_bloque(int direccion_bloque, void* buffer, int tamano_restante_ultimo_bloque) {
  return 0;
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

void* buffer_desde(void* buffer_original, int tamano) {
  void* buffer_salida = NULL;
  printf("size %lu\n", sizeof(void*));

  return buffer_salida;
}
