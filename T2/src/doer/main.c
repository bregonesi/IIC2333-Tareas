#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  /* El programa recibe 2 parametros */
	if(argc != 3)
	{
		printf("Modo de uso: %s <file> <n>\n", argv[0]);
		return 1;
	}

  char* input_file = argv[1];
	FILE *archivo_tareas;
	archivo_tareas = fopen(input_file, "r");	// leyendo archivo de input

	if (!(archivo_tareas)) {
		printf("Archivo %s no existe.\n", input_file);
		return 1;
	}

  /* Lectura de archivo */
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, archivo_tareas)) != -1) {
    printf("Retrieved line of length %zu :\n", read);
    printf("%s", line);

		// commando: find . -name "tests.txt"
		//char *args[]={"find", ".", "-name", "tests.txt", NULL}; // este es el caso

		// commando date "+DATE: %Y-%m-%d%nTIME: %H:%M:%S"
		char *args[]={"date", "+DATE: %Y-%m-%d%nTIME: %H:%M:%S", NULL}; // este es el caso
		execvp(args[0],args);
  }
  /* */

  if (line) free(line);

  fclose(archivo_tareas);

  printf("Este es el main de doer\n");

}
