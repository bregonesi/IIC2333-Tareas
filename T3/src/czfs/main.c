#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(int argc, char *argv[])
{
  if(argc != 1)
	{
		printf("Modo de uso: %s <disk_file>\n", argv[0]);
		return 1;
	}

  char* disco_file = argv[2];

  return 0;
}
