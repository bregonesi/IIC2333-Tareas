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
      int indice;
      fread(&indice, 4, 1, fp);

      if(valid[0] == 1 && strcmp(name, filename) == 0 && !bitmap_entry_is_free(indice)) { // si existe
        //printf("indice en directorio: %i\n", indice);

        file = malloc(sizeof(czFILE));

        file->indice_en_directorio = i;
        file->direccion_bloque = indice * 1024; //direccion del bloque indice

        file->nombre = name;

        fseek(fp, file->direccion_bloque, SEEK_SET);  // nos vemos al archivo

        fread(&file->tamano, 4, 1, fp);
        fread(&file->creacion, 4, 1, fp);
        fread(&file->modificacion, 4, 1, fp);

        fseek(fp, 1008, SEEK_CUR);  // nos saltamos la data
        fread(&file->next_bloque, 4, 1, fp);

        file->modo = 'r';
        file->closed = false;
        file->ultimo_byte_leido = 0;

        int tamano_datos = file->tamano - 12; //incluye los punteros
        if(tamano_datos >= 252 * (4 + 1024))  // es decir, si tiene direccionamiento indirecto
          tamano_datos -= 4;  // le quitamos los bytes donde esta la tabla de direccionamiento indirecto

        int offset_datos = tamano_datos % 1028;  // sacamos el ultimo bloque
        int cantidad_bloques = (tamano_datos - offset_datos) / 1028;  // esto nos dice cuantos bloques estan al 100%
        if(cantidad_bloques == 0 && tamano_datos != 0)  // es decir, si solo tenemos el primer bloque un poco usado
          offset_datos -= 4;
        file->tamano_datos = (1024 * cantidad_bloques) + offset_datos;

        fclose(fp);
        free(valid);

        return file;
      }
      i += 16;
      free(name);
      free(valid);
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
        fwrite((const void*) &n_bloque, 4, 1, fp);

        /* Nos metemos al bloque del archivo */
        fseek(fp, n_bloque * 1024, SEEK_SET);

        int tamano = 12;  // 12 bytes de metadata
        fwrite((const void*) &tamano, 4, 1, fp);

        time_t t_creacion = time(NULL);
        fwrite((const void*) &t_creacion, 4, 1, fp);  // T creacion
        fwrite((const void*) &t_creacion, 4, 1, fp);  // T modificacion

        file = malloc(sizeof(czFILE));
        file->indice_en_directorio = i;
        file->direccion_bloque = n_bloque * 1024;
        file->nombre = malloc(sizeof(char) * 11);
        strcpy(file->nombre, filename);
        file->modo = 'w';
        file->closed = false;
        file->ultimo_byte_leido = 0;
        file->tamano = 12; //solamente el metadata
        file->tamano_datos = 0; // 0 punteros escritos
        file->creacion = t_creacion;
        file->modificacion = t_creacion;
        file->next_bloque = 0;

        //printf("archivo %s creado en bloque %i\n", filename, n_bloque);

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

void cz_ls() {
  FILE* fp = fopen(ruta_bin, "rb");

  //printf("Printeando ls\n");

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

  for(int k = 0; k < 9; k++) {  // los primeros 9 son directorio y bitmap
    bitmap_set_first();
  }
}

int cz_close(czFILE* file_desc) {
  file_desc->closed = true;
  return 0;
}

int cz_mv(char* orig, char *dest) {
  if (!cz_exists(orig) || cz_exists(dest)) {  // ver si orig existe y si dest no existe
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

    if(valid[0] == 1 && !bitmap_entry_is_free(indice) && (strcmp(orig, name) == 0)) {
      //printf("antiguo: '%s', cambiado a: '%s'\n", orig, dest);
      fseek(fp, i + 1, SEEK_SET);  // nos movemos al nombre
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

int cz_write(czFILE* file_desc, void* buffer, int nbytes) {
  //printf("entrando a escribi -------------------r\n");
  if(file_desc->modo == 'w' && !file_desc->closed) {
    int bytes_escribir = MIN((252 + 256) * 1024 - file_desc->tamano_datos, nbytes);  // maximo que puede escribir
    //printf("vamos a escribir %i bytes\n", bytes_escribir);

    int tamano_restante_ultimo_bloque = 1024 - file_desc->tamano_datos % 1024;
    //printf("tamaño restante en ultimo bloque: %i\n", tamano_restante_ultimo_bloque);

    int cantidad_bloques_nuevos = (bytes_escribir - tamano_restante_ultimo_bloque) / 1024;
    if(tamano_restante_ultimo_bloque == 1024)  // si el ultimo bloque lo ocupamos 100%, entonces usaremos uno nuevo
      cantidad_bloques_nuevos += 1;
    //printf("tamano restante ultimo bloque %i, bloques nuevos %i\n", tamano_restante_ultimo_bloque, cantidad_bloques_nuevos);

    //printf("tamano archivo antes: %i\n", file_desc->tamano);
    file_desc->tamano += (4 * cantidad_bloques_nuevos) + bytes_escribir; // no se considera fragmentacion interna
    //printf("tamano archivo despues: %i\n", file_desc->tamano);

    int n_bloque = (file_desc->tamano_datos - file_desc->tamano_datos % 1024) / 1024;
    //printf("----vamos a escribir desde el bloque %i\n", n_bloque);
    //printf("tamano datos %i\n", file_desc->tamano_datos);

    //printf("bytes a escribir: %i\n", bytes_escribir);
    //printf("tamano solo datos antes: %i\n", file_desc->tamano_datos);
    file_desc->tamano_datos += bytes_escribir;
    //printf("tamano solo datos despues: %i\n", file_desc->tamano_datos);

    time_t t_modificacion = time(NULL);
    file_desc->modificacion = t_modificacion;
    //printf("t modificacion %i\n", (int)file_desc->modificacion);

    FILE* fp = fopen(ruta_bin, "rb+");

    //printf("escribimos tamano %i y t modificacion %i\n", file_desc->tamano, (int)file_desc->modificacion);
    fseek(fp, file_desc->direccion_bloque, SEEK_SET);  // vamos a la direcion en directorio
    fwrite((const void*) &file_desc->tamano, 4, 1,fp);
    fseek(fp, 4, SEEK_CUR);  // nos saltamos la creacion
    fwrite((const void*) &file_desc->modificacion, 4, 1, fp);  // T modificacion

    //printf("escibiendo: %s\n", buffer);
    int direccion_bloque;
    if(n_bloque < 252)  // si escribimos en los bloques directos
      direccion_bloque = file_desc->direccion_bloque + 12 + (n_bloque * 4);
    else  // si escribimos en los bloques indirectos | si next_bloque no esta seteado se setea cuando se entra al while y se recalcula direccion_bloque
      direccion_bloque = file_desc->next_bloque + ((n_bloque - 252) * 4);
    //printf("direccion bloque datos a escribir: %i\n", direccion_bloque);

    int bytes_restantes = nbytes;
    int sum_bytes_escritos = 0;
    while(bytes_restantes > 0 && n_bloque < 508) {  // el primero va de 0 a 251 y el segundo de 252 a 252 + 256 = 508
      if(tamano_restante_ultimo_bloque == 0) {  // si se nos acaba bloque hay que hacer uno nuevo
          n_bloque++;
          direccion_bloque += 4;
          tamano_restante_ultimo_bloque = 1024;

          fseek(fp, file_desc->direccion_bloque, SEEK_SET);  // vamos al tamano del archivo
          file_desc->tamano += 4;  // le sumamos los bytes de direccion del bloque indirecto
          fwrite((const void*) &file_desc->tamano, 4, 1, fp);

          // el encargado de hacer el ultimo bloque y guardarlo es un if de mas abajo
      }

      //printf("n bloque %i next bloque %i\n", n_bloque, file_desc->next_bloque);
      if(n_bloque >= 252 && file_desc->next_bloque == 0) {  // seteamos la direccion de punteros indirectos
        file_desc->next_bloque = (bitmap_set_first() - 1024) * 1024;
        direccion_bloque = file_desc->next_bloque + ((n_bloque - 252) * 4);

        fseek(fp, file_desc->direccion_bloque + 12 + 1008, SEEK_SET);  // vamos a la direccion del bloque indirecto
        fwrite((const void*) &file_desc->next_bloque, 4, 1, fp);
        //printf("Guardando direccion de bloque indirecto. Nuevo tamano archivo %i, direccion %i\n", file_desc->tamano, (int)file_desc->next_bloque);
        //printf("Seteando direccion bloque a %i\n", direccion_bloque);
      }

      int direccion_bloque_datos;
      fseek(fp, direccion_bloque, SEEK_SET);  // nos vamos al bloque
      fread(&direccion_bloque_datos, 4, 1, fp);  // y leemos que bloque tiene asignado

      if((tamano_restante_ultimo_bloque == 1024 && bitmap_entry_is_free(direccion_bloque_datos))
        || (direccion_bloque_datos >= 0 && direccion_bloque_datos <= 9)) {  // tenemos que hacer una nueva entrada si es que no existe en bitmap o tiene asignado los reservados
        direccion_bloque_datos = (bitmap_set_first() - 1024) * 1024;
        fseek(fp, direccion_bloque, SEEK_SET);  // nos vamos al bloque
        fwrite((const void*) &direccion_bloque_datos, 4, 1, fp);  // escribimos el bloque asignado
      }

      int bytes_a_escribir = MIN(tamano_restante_ultimo_bloque, bytes_restantes);
      fseek(fp, direccion_bloque_datos, SEEK_SET);  // nos posicionamos en el bloque a escribir
      //printf("Vamos a escribir %i bytes en %i\n", bytes_a_escribir, direccion_bloque_datos);

      fseek(fp, 1024 - tamano_restante_ultimo_bloque, SEEK_CUR);  // movemos al puntero a lo que queda
      //printf("movemos el puntero a %i\n", 1024 - tamano_restante_ultimo_bloque);

      //escribiendo aca
      fwrite(buffer, bytes_a_escribir, 1, fp);
      //printf("antigu buffer: %s\n", buffer);
      buffer = buffer + bytes_a_escribir;
      //printf("nuevo buffer: %s\n", buffer);
      //

      tamano_restante_ultimo_bloque -= bytes_a_escribir;
      sum_bytes_escritos += bytes_a_escribir;  // correctamente deberia ir en el retorno de fwrite, pero no calza nose pq
      bytes_restantes -= bytes_a_escribir;
      //printf("bytes restantes: %i\n", bytes_restantes);
      //printf("sum bytes escritos %i\n", sum_bytes_escritos);
    }

    fclose(fp);

    return sum_bytes_escritos;  // retornando los bytes que se escribieron
  }
  return -1;
}


int cz_read(czFILE* file_desc, void* buffer, int nbytes) {
  if(file_desc->modo == 'w' || file_desc->closed) {  // esta un poco demas este if, pero mejor que este para asegurarse
    return -1;
  }


  if(file_desc->modo == 'r' && !file_desc->closed) {
    //printf("entrando a leer\n");

    int bytes_leer = MIN(file_desc->tamano_datos - file_desc->ultimo_byte_leido, nbytes);
    //printf("vamos a leer %i bytes\n", bytes_leer);

    int sum_bytes_leidos = 0;
    int datos_restantes = file_desc->tamano_datos - file_desc->ultimo_byte_leido;  //del archivo entero
    int bytes_leer_restantes = bytes_leer;

    int offset_bloque;
    int n_bloque;
    int direccion_bloque;
    int direccion_bloque_datos;
    int cantidad_a_leer_en_bloque;

    FILE* fp = fopen(ruta_bin, "rb");
    while(bytes_leer_restantes > 0 && datos_restantes > 0) {
      //printf("entrando al while. bytes leer restantes %i\n", bytes_leer_restantes);

      offset_bloque = file_desc->ultimo_byte_leido % 1024;
      n_bloque = (file_desc->ultimo_byte_leido - offset_bloque) / 1024;

      if(n_bloque < 252)  // si leemos en los bloques directos
        direccion_bloque = file_desc->direccion_bloque + 12 + (n_bloque * 4);
      else  // si leemos en los bloques indirectos
        direccion_bloque = file_desc->next_bloque + ((n_bloque - 252) * 4);

      fseek(fp, direccion_bloque, SEEK_SET);  // buscamos la direccion del bloque
      fread(&direccion_bloque_datos, 4, 1, fp);
      fseek(fp, direccion_bloque_datos, SEEK_SET);  // nos vamos a la direccion del bloque
      //printf("direccion exacta de datos a leer: %i\n", direccion_bloque_datos);

      fseek(fp, offset_bloque, SEEK_CUR);  // nos movemos en el offset
      //printf("offset: %i\n", offset_bloque);

      if(datos_restantes < 1024)
        cantidad_a_leer_en_bloque = MIN(bytes_leer_restantes, datos_restantes);
      else
        cantidad_a_leer_en_bloque = MIN(bytes_leer_restantes, 1024 - offset_bloque);

      //printf("cantidad_a_leer_en_bloque: %i\n", cantidad_a_leer_en_bloque);

      fread(buffer + sum_bytes_leidos, 1, cantidad_a_leer_en_bloque, fp);
      sum_bytes_leidos += cantidad_a_leer_en_bloque;
      file_desc->ultimo_byte_leido += cantidad_a_leer_en_bloque;
      bytes_leer_restantes -= cantidad_a_leer_en_bloque;
      datos_restantes = file_desc->tamano_datos - file_desc->ultimo_byte_leido;

    }


    fclose(fp);

    return sum_bytes_leidos;  // retornando los bytes que se escribieron
  }
  return -1;
}

int cz_cp(char* orig, char* dest) {
  // vamos a leer por bloque y escribir por bloque para que no tengamos problemas

  if(!cz_exists(orig) || cz_exists(dest))  // ver si orig existe y si dest no existe
    return -1;

  czFILE* file_orig = cz_open(orig, 'r');
  czFILE* file_dest = cz_open(dest, 'w');

  int bytes_restantes = file_orig->tamano_datos;
  void* buffer = NULL;
  while(bytes_restantes > 0) {
    //printf("bytes restantes %i\n", bytes_restantes);
    int bytes_escribir = MIN(bytes_restantes, 1024);
    buffer = calloc(bytes_escribir + 1, sizeof(void));
    cz_read(file_orig, buffer, bytes_escribir);
    cz_write(file_dest, buffer, bytes_escribir);
    free(buffer);
    bytes_restantes -= bytes_escribir;
  }

  cz_free(file_orig);
  cz_free(file_dest);

  return 0;
}

int cz_rm(char* filename) {
  if(!cz_exists(filename))  // ver si el archivo existe
    return -1;

  czFILE* file = cz_open(filename, 'r');

  printf("indice directorio %i\n", file->indice_en_directorio);

  FILE* fp = fopen(ruta_bin, "rb+");
  fseek(fp, file->indice_en_directorio, SEEK_SET);  // vamos al directorio
  char* valid = calloc(2, sizeof(char));
  valid[0] = 0;
  fwrite(valid, 1, 1, fp);  // dejamos valid en 0 en directorio
  free(valid);

  fclose(fp);
  cz_free(file);
  return 0;
}

void cz_free(czFILE* file) {
  free(file->nombre);
  free(file);
}
