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

        int n_bloque = bitmap_set_first() - 1024; // setea en bitmap el bloque a usar y se guarda la posicion en disco asignada
        fwrite((const void*) & n_bloque,sizeof(int),1,fp);

        /* Nos metemos al bloque del archivo */
        fseek(fp, n_bloque * 1024, SEEK_SET);

        int tamano = 12;  // 12 bytes de metadata
        fwrite((const void*) & tamano,sizeof(int),1,fp);

        char* creacion = calloc(5, sizeof(char));
        itoa(T, creacion, 10);  // T es nuestra varaible global
        fwrite(creacion, 4, 1, fp);

        char* modificacion = calloc(5, sizeof(char));
        itoa(T, modificacion, 10);  // T es nuestra varaible global
        fwrite(modificacion, 4, 1, fp);

        file = malloc(sizeof(czFILE));
        file->indice_en_directorio = i;
        file->direccion_bloque = n_bloque * 1024;
        file->nombre = malloc(sizeof(char) * 11);
        strcpy(file->nombre, filename);
        file->modo = 'w';
        file->closed = false;
        file->tamano = 12; //solamente el metadata
        file->tamano_datos = 0; // 0 punteros escritos
        file->creacion = T;
        file->modificacion = T;

        free(creacion);
        free(modificacion);

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
    int indice;
    fread(&indice, sizeof(int), 1, fp);

    if(valid[0] == 1 && strcmp(name, filename) == 0 && !bitmap_entry_is_free(indice)) {
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
    printf("vamos a escribir %i bytes\n", bytes_escribir);

    int tamano_restante_ultimo_bloque = file_desc->tamano_datos % 1024;
    int cantidad_bloques_nuevos = (bytes_escribir - tamano_restante_ultimo_bloque)/1024;
    if(file_desc->tamano_datos == 0)
      cantidad_bloques_nuevos += 1;
    printf("tamano restante %i, bloques nuevos %i\n", tamano_restante_ultimo_bloque, cantidad_bloques_nuevos);

    file_desc->tamano += bytes_escribir + 4 * cantidad_bloques_nuevos;
    file_desc->modificacion = T;  // T es nuestra variable global

    FILE* fp = fopen(ruta_bin, "rb+");

    fseek(fp, file_desc->direccion_bloque, SEEK_SET);
    int tamano = file_desc->tamano;
    printf("%i\n", tamano);
    fwrite((const void*) & tamano,sizeof(int),1,fp);

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
    void* buffer_sobra = buffer;
    while(bytes_escribir > 0) {  // en realidad != 0
      printf("faltan %i bytes para terminar\n", bytes_escribir);
      direccion_bloque = file_desc->direccion_bloque + 12 + n_bloque * 4;
      if(n_bloque > 251)  // del 0 al 251 va en el espacio de 1008 bytes, del 252 en adelante van en direccionamiento indirecto
        direccion_bloque = file_desc->next_bloque + (n_bloque - 251) * 4;  // le sumo los 1008 + el inicio del otro bloque

      if(tamano_restante_ultimo_bloque > 0) {  // rellenamos ultimo bloque
        //bytes_escritos = cz_write_bloque(direccion_bloque, buffer_sobra, tamano_restante_ultimo_bloque);
        bytes_escritos = tamano_restante_ultimo_bloque;
        fseek(fp, direccion_bloque, SEEK_SET);
        int direccion_bloque_dato = (bitmap_set_first() - 1024) * 1024;
        fwrite((const void*) & direccion_bloque_dato,sizeof(int),1,fp);
        fseek(fp, direccion_bloque_dato, SEEK_SET);
        fwrite(buffer_sobra, tamano_restante_ultimo_bloque, 1, fp);
      } else {  //nuevo bloque
        int bytes_escribir_bloque_n = MIN(1024, bytes_escribir);
        printf("ddddd %i\n", bytes_escribir_bloque_n);
        //bytes_escritos = cz_write_bloque(direccion_bloque, buffer_sobra, bytes_escribir_bloque_n);
        bytes_escritos = bytes_escribir_bloque_n;
        fseek(fp, direccion_bloque, SEEK_SET);
        int direccion_bloque_dato = (bitmap_set_first() - 1024) * 1024;
        printf("jfslkdjflk %i\n", direccion_bloque_dato);
        fwrite((const void*) & direccion_bloque_dato,sizeof(int),1,fp);
        fseek(fp, direccion_bloque_dato, SEEK_SET);
        fwrite(buffer_sobra, bytes_escribir_bloque_n, 1, fp);
      }
      //buffer_sobra = buffer_desde(buffer, bytes_escritos);
      sum_bytes_escritos += bytes_escritos;
      bytes_escribir -= bytes_escritos;
      file_desc->tamano_datos += bytes_escritos;
      tamano_restante_ultimo_bloque -= bytes_escritos;
      if(tamano_restante_ultimo_bloque == 0) {
        n_bloque++;
        tamano_restante_ultimo_bloque = 1024;
      }
      if(n_bloque == 251) {
        file_desc->tamano += 4;
        // hacer fwrite del tamano
        //file_desc->direccion_bloque
        // hacer fwrite del bloque
        printf("hay que setear direccion para direccionamiento indirecto y incrementar tamaño del archivo\n");
      }
    }
    // hacer fwrite del nuevo tamaño

    fclose(fp);

    return sum_bytes_escritos;  // cambiar a retornar bytes escritos
  }
  return -1;
}

void cz_ls() {
  FILE* fp = fopen(ruta_bin, "rb");

  printf("Printeando ls\n");

  int i = 0;
  while(i < 1024) {
    char* valid = calloc(2, sizeof(char));
    fread(valid, 1, 1, fp);
    char* name = calloc(12, sizeof(char));
    fread(name, 11, 1, fp);
    int indice;
    fread(&indice, sizeof(int), 1, fp);

    if(valid[0] == 1 && !bitmap_entry_is_free(indice))
      printf("%s\n", name);

    free(valid);
    free(name);

    i += 16;
  }

  fclose(fp);
}

void cz_mount(char* diskfileName) {
  ruta_bin = diskfileName;

  for(int k = 0; k < 3; k++) {
    bitmap_set_first();
  }
}

int cz_write_bloque(int direccion_bloque, void* buffer, int tamano_restante_ultimo_bloque) {

  return 0;
}

int cz_close(czFILE* file_desc) {
  file_desc -> closed = 1;
  return 0;
}

int cz_mv(char* orig, char *dest){
  //ver si dest ya existe, si existe ret 1
  if (cz_exists(dest)) {
    return 1;
  }
  else if (!cz_exists(orig)) { //por si orig no existe
    return 1;
  }
  // en caso de que dest no existe y orig si existe:
  FILE* fp = fopen(ruta_bin, "rb+");

  int i = 0;
  while(i < 1024) {
    char* valid = calloc(2, sizeof(char));
    fread(valid, 1, 1, fp);
    char* name = calloc(12, sizeof(char));
    fread(name, 11, 1, fp);
    int indice;
    fread(&indice, sizeof(int), 1, fp);

    if(valid[0] == 1 && !bitmap_entry_is_free(indice) && (strcmp(orig, name) == 0)){
      printf("antiguo: '%s', cambiado a: '%s'\n", orig, dest);
      fseek(fp, i + 1, SEEK_SET);
      fwrite(dest, 11, 1, fp);
      free(valid);
      free(name);
      fclose(fp);
      return 0;
    }

    free(valid);
    free(name);

    i += 16;
  }

  fclose(fp);
  return 0;
}
