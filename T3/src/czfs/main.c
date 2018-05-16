#include "cz_API.h"
#include <time.h>

int main(int argc, char *argv[])
{
  if(argc != 2)
	{
		printf("Modo de uso: %s <disk_file>\n", argv[0]);
		return 1;
	}

  cz_mount(argv[1]);

  if (!(ruta_bin)) {
    printf("Archivo %s no existe.\n", ruta_bin);
    return 1;
  }

/*FILE* fp = fopen(ruta_bin, "rb+");

fseek(fp, 0, SEEK_SET);
fwrite("1", 1, 1, fp);
char a[4] = "hola";
fwrite(a, sizeof(char), 4, fp);
fclose(fp);*/
cz_ls();
printf("existe 'input.txt'?: %i\n", cz_exists("input.txt"));

//char* b = calloc(2, sizeof(char));
for(int k = 0; k < 200; k++) {
  FILE* fp = fopen(ruta_bin, "rb+");
  fseek(fp, 1023 , SEEK_SET);
  //b[0] = k;
  //fwrite(b, 1, 1, fp);
  fclose(fp);
  int posicion_fisica_des = bitmap_set_first();
  printf("bit fisico ocupado: %i\n", posicion_fisica_des);
  printf("numero de bloque ocupado: %i\n", posicion_fisica_des - 1024 );
}
//free(b);
czFILE* file_new = NULL;
file_new = cz_open("test.txt", 'w');
buffer_desde("hola", 3);
//cz_write(file_new, "hola", 4);

free(file_new->nombre);
free(file_new);
bitmap_de_bloque(1029);


  return 0;
}
