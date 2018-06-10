#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "mensajes.h"
#include "funciones.h"

int main(int argc, char *argv[]) {
  if(argc != 5) {
    printf("Modo de uso: %s [-i <ip_address>] [-p <tcp-port>]\n", argv[0]);
    return 1;
  }

  char* server_address = NULL;
  int port = -1;
  for(int i = 0; i < argc; i++) {
    if(strcmp(argv[i], "-i") == 0)
      server_address = argv[i + 1];
    if(strcmp(argv[i], "-p") == 0)
      port = atoi(argv[i + 1]);
  }

  if(server_address == NULL || port == -1) {  // si nos entregaron los parametros incorrectos
    printf("Modo de uso: %s [-i <ip_address>] [-p <tcp-port>]\n", argv[0]);
    return 1;
  }

  printf("Address %s, Puerto: %i\n", server_address, port);

  int sock = 0;
  struct sockaddr_in serv_addr;
  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);
  if(inet_pton(AF_INET, server_address, &serv_addr.sin_addr)<=0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }

  char buffer[2057];  // 8 + 8 + 8*255 + 1
  char* mensaje_enviar;
  char** mensaje_recibir;  // [mensaje_id, tamano, mensaje]

  // Enviar mensaje de start conection
  mensaje_enviar = codificar(start_conection, "");
  send(sock, mensaje_enviar, strlen(mensaje_enviar), 0);
  free_codificacion(mensaje_enviar);

  read(sock, buffer, 16);  // si bien el buffer es de 2057, hay que usar 16 si no se envia texto, sino se traspapelan los buffers
  mensaje_recibir = decodificar(buffer);
  if(atoi(mensaje_recibir[0]) == conection_established) {  // es decir, si recibo id de conection established
    printf("Recibo confirmacion\n");
  } else {
    // quizas estos errores hay que manejarlos diferente
    perror("no se recibio mensaje de conexion establecida");
    exit(EXIT_FAILURE);
  }
  free_decodificacion(mensaje_recibir);

  read(sock, buffer, 16);
  mensaje_recibir = decodificar(buffer);
  if(atoi(mensaje_recibir[0]) != ask_nickname) {  // es decir, si no recibo id de ask nickname
    // quizas estos errores hay que manejarlos diferente
    perror("no se recibio mensaje de ask nickname");
    exit(EXIT_FAILURE);
  }

  printf("%s\n", mensajes[atoi(mensaje_recibir[0])]);
  free_decodificacion(mensaje_recibir);

  char input_text[255];
  fgets(input_text, 255, stdin);
  input_text[strlen(input_text) - 1] = '\0';  // sino queda con \n el final
  mensaje_enviar = codificar(return_nickname, input_text);
  send(sock, mensaje_enviar, strlen(mensaje_enviar), 0);
  free_codificacion(mensaje_enviar);

  read(sock, buffer, 2057);
  mensaje_recibir = decodificar(buffer);
  if(atoi(mensaje_recibir[0]) != opponent_found) {
    // quizas estos errores hay que manejarlos diferente
    perror("no se recibio mensaje de opponent found");
    exit(EXIT_FAILURE);
  }
  printf("%s %s\n", mensajes[atoi(mensaje_recibir[0])], mensaje_recibir[2]);
  free_decodificacion(mensaje_recibir);

  read(sock, buffer, 2057);
  mensaje_recibir = decodificar(buffer);
  if(atoi(mensaje_recibir[0]) != initial_pot) {
    // quizas estos errores hay que manejarlos diferente
    perror("no se recibio mensaje de initial pot");
    exit(EXIT_FAILURE);
  }
  printf("%s %s\n", mensajes[atoi(mensaje_recibir[0])], mensaje_recibir[2]);
  free_decodificacion(mensaje_recibir);

  read(sock, buffer, 2057);
  mensaje_recibir = decodificar(buffer);
  if(atoi(mensaje_recibir[0]) != game_start) {
    // quizas estos errores hay que manejarlos diferente
    perror("no se recibio inicio de partida");
    exit(EXIT_FAILURE);
  }
  printf("%s\n", mensajes[atoi(mensaje_recibir[0])]);
  free_decodificacion(mensaje_recibir);

  int jugando = 1;
  while(jugando == 1) {
    read(sock, buffer, 2057);
    mensaje_recibir = decodificar(buffer);
    if(atoi(mensaje_recibir[0]) != start_round && atoi(mensaje_recibir[0]) != game_end) {
      // quizas estos errores hay que manejarlos diferente
      perror("no se recibio inicio o termino de ronda");
      exit(EXIT_FAILURE);
    }

    if(atoi(mensaje_recibir[0]) == game_end) {
      free_decodificacion(mensaje_recibir);
      
      jugando = 0;
      printf("%s\n", mensajes[atoi(mensaje_recibir[0])]);
      free_decodificacion(mensaje_recibir);
    }
    else {  // start_round
      free_decodificacion(mensaje_recibir);

      read(sock, buffer, 2057);
      mensaje_recibir = decodificar(buffer);
      if(atoi(mensaje_recibir[0]) != initial_bet) {
        // quizas estos errores hay que manejarlos diferente
        perror("no se recibio initial bet");
        exit(EXIT_FAILURE);
      }
      printf("%s\n", mensajes[atoi(mensaje_recibir[0])]);
      free_decodificacion(mensaje_recibir);
    }
  }

  return 0;
}
