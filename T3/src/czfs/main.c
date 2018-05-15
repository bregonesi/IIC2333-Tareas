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
/*FILE* fp = fopen(ruta_bin, "rb+");

fseek(fp, 0, SEEK_SET);
fwrite("1", 1, 1, fp);
char a[4] = "hola";
fwrite(a, sizeof(char), 4, fp);
fclose(fp);*/
printf("%zu\n", sizeof(char));
  cz_ls();
char b = 'c';
printf("%i\n", b);
char b_bin[8];
itoa(b, b_bin, 2);
char* fill_b = fill_binario(b_bin, 8);
printf("%s\n", fill_b);
free(fill_b);


printf("%i\n", bitmap_get_free());
FILE* fp = fopen(ruta_bin, "rb+");
for(int k = 0; k < 30; k++) {
fseek(fp, 1024 + k, SEEK_SET);
fwrite("1", 1, 1, fp);
printf("%i\n", bitmap_get_free());
fwrite("11", 1, 1, fp);
printf("%i\n", bitmap_get_free());
fwrite("111", 1, 1, fp);
printf("%i\n", bitmap_get_free());
fwrite("1111", 1, 1, fp);
printf("%i\n", bitmap_get_free());
fwrite("11111", 1, 1, fp);
printf("%i\n", bitmap_get_free());
fwrite("111111", 1, 1, fp);
printf("%i\n", bitmap_get_free());
fwrite("1111111", 1, 1, fp);
printf("%i\n", bitmap_get_free());
fwrite("11111111", 1, 1, fp);
printf("%i\n", bitmap_get_free());
}
fclose(fp);
  return 0;
}
