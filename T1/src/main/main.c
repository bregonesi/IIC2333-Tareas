#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	/* El programa recibe 3 parametros */
	if(argc != 2)
	{
		printf("Modo de uso: %s <input.png> <kernel.txt> <output.png>\n", argv[0]);
		return 1;
	}

  printf("%s\n", argv[1]);

	return 0;
}
