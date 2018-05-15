#include "cz_API.h"


int main(int argc, char *argv[])
{
  if(argc != 1)
	{
		printf("Modo de uso: %s <disk_file>\n", argv[0]);
		return 1;
	}

  ruta_bin = argv[1];

  return 0;
}
