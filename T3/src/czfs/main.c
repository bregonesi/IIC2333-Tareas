#include "cz_API.h"
#include <time.h>

int main(int argc, char *argv[])
{
  if(argc != 2)
	{
		printf("Modo de uso: %s <disk_file>\n", argv[0]);
		return 1;
	}

  ruta_bin = argv[1];

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

char* b = calloc(2, sizeof(char));
FILE* fp = fopen(ruta_bin, "rb+");
for(int k = 0; k < 1; k++) {
  fseek(fp, 0 + k, SEEK_SET);
  b[0] = 1;
  printf("b %s, %i\n", b, b[0]);
  fwrite(b, 1, 1, fp);
  printf("%i\n", bitmap_get_free());
  b[0] = 3;
  fwrite(b, 1, 1, fp);
  printf("b %s, %i\n", b, b[0]);
  printf("%i\n", bitmap_get_free());
  b[0] = 5;
  fwrite(b, 1, 1, fp);
  printf("b %s, %i\n", b, b[0]);
  printf("%i\n", bitmap_get_free());
  b[0] = 13;
  fwrite(b, 1, 1, fp);
  printf("b %s, %i\n", b, b[0]);
  printf("%i\n", bitmap_get_free());
}
printf("printeando\n");
char ll[4];
fseek(fp, 0, SEEK_SET);
fread(ll, 4, 1, fp);
printf("%i,%i,%i,%i\n", ll[0],ll[1],ll[2],ll[3]);

fclose(fp);
  return 0;
}
