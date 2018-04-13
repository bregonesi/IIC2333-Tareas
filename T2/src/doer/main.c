#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

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

	char line[1024];
	char ch = getc(archivo_tareas);
  int index = 0;
	int params = 0;
	bool colon_param = false;
	char **args = malloc(sizeof(char*) * 2);
	while ( ch != EOF ) {
		if(ch != '\n') {  // vamos guardando el parametro en linea caracter por caracter
			line[index++] = ch;
    }

		if(ch == '"') {  // si hay algun parametro que use comillas
			if(colon_param) colon_param = false;
			else colon_param = true;
		}

		if((ch == ' ' || ch == '\n') && !colon_param) {  // guardamos nuevo parametro
	      line[index] = '\0';
	      index = 0;
				params++;

				if(params != 1) {  // hay que expandir args
					printf("realloc\n");
					args = realloc(args, sizeof(char*) * (params + 1));
				}

				printf("fds %i\n", params-1);
				strcpy(args[params - 1], "hola\0");  // esto esta fallando
				// strcpy(args[params - 1], line)  // aqui se pasa a una lista para que sea ejecutado por execvp
				printf("%s\n", args[params-1]);

				printf("jfldjf %s: %i\n", line, params);

				if(ch == '\n') {
					printf("fin\n");
					for(int i = 0; i < params; i++) {
						printf("%s\n", args[i]);
					}
					args[params] = NULL;
					execvp(args[0],args);
				}
		}

		if(ch == '\n') { // por si empieza nueva tarea
			params = 0;
			args = malloc(sizeof(char*) * 2);
		}

		ch = getc(archivo_tareas);
  }


  /*char *line = NULL;
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
  }*/
  /* */

  //if (line) free(line);

  fclose(archivo_tareas);

  printf("Este es el main de doer\n");

}
