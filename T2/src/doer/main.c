#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  /* El programa recibe 2 parametros */
	if(argc != 3)
	{
		printf("Modo de uso: %s <file> <n>\n", argv[0]);
		return 1;
	}
  
  printf("Este es el main de doer\n");

}
