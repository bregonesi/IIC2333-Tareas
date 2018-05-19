#include "cz_API.h"

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
//cz_ls();
//printf("existe 'input.txt'?: %i\n", cz_exists("input.txt"));

//char* b = calloc(2, sizeof(char));

//free(b);
czFILE* file_new = cz_open("test.txt", 'w');

//buffer_desde("hola", 3);
printf("-------\n");
cz_write(file_new, "hola", 4);
//sleep(1);  // para q cambie el t de modificacion
//for(int i = 0; i < 100; i++)  // asi escribimos infinitas weas
for(int i = 0; i < 23458; i++)  // asi escribimos infinitas weas
  cz_write(file_new, "chao1234567", 11);
//sleep(1);  // para q cambie el t de modificacion
cz_write(file_new, "asdf", 4);
cz_write(file_new, "asdf", 4);

printf("cantidad de datos %i\n", file_new->tamano_datos);

cz_close(file_new);
cz_free(file_new);

czFILE* file_new_2 = cz_open("test.txt", 'r');

void* buffer_lectura = calloc(22, sizeof(void));
int bytes_leidos = cz_read(file_new_2, buffer_lectura, 4);
//printf("Read: %s\n", buffer_lectura); //tira error
printf("Bytes leidos: %i\n", bytes_leidos);
printf("-------\n");
printf("%s\n", buffer_lectura);
bytes_leidos = cz_read(file_new_2, buffer_lectura, 4);
printf("%s\n", buffer_lectura);
free(buffer_lectura);



buffer_lectura = calloc(101, sizeof(void));
bytes_leidos = cz_read(file_new_2, buffer_lectura, 100);
//printf("Read: %s\n", buffer_lectura); //tira error
printf("Bytes leidos: %i\n", bytes_leidos);
printf("-------\n");
printf("%s\n", buffer_lectura);
free(buffer_lectura);
/*printf("TODOS LOS ARCHIVOS: \n");
cz_ls();
printf("existe test? %i\n", cz_exists("test.txt"));
cz_mv("test.txt", "otro.txt");
printf("TODOS LOS ARCHIVOS: \n");
cz_ls();
printf("existe test? %i\n", cz_exists("test.txt"));*/

cz_free(file_new_2);

printf("orig %i\n", cz_exists("test.txt"));
printf("dest %i\n", cz_exists("hola.txt"));
int a = cz_cp("test.txt", "hola.txt");
printf("a %i\n", a);

cz_mv("test.txt", "shola.txt");
cz_rm("shola.txt");
//bitmap_de_bloque(1029);
/*
FILE* fp = fopen(ruta_bin, "rb+");

int n_bloque;
fseek(fp, 12, SEEK_SET);
fread(&n_bloque, sizeof(int), 1, fp);
printf("n_bloque leido: %i\n", n_bloque);
fclose(fp);
*/


  return 0;
}
