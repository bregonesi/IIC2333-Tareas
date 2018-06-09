#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "mensajes.h"
#include "funciones.h"
#include "cartas.h"

int main(int argc, char *argv[]) {
  Mazo* mazo_juego = crear_mazo();
  print_mazo(*mazo_juego);
  int* carta = sacar_carta(mazo_juego);
  free(carta);
  print_mazo(*mazo_juego);

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

  int server_fd;  // socket del server
  int opt = 1;  // ni idea q es esto
  struct sockaddr_in address;  // info de conexion
  int addrlen = sizeof(address);

  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  //address.sin_addr.s_addr = INADDR_ANY;
  address.sin_addr.s_addr = inet_addr(server_address);
  address.sin_port = htons(port);

  if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if(listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  char buffer[2057];  // 8 + 8 + 8*255 + 1
  char* mensaje_enviar;
  char** mensaje_recibir;  // [mensaje_id, tamano, mensaje]

  int* clientes = calloc(2, sizeof(int));
  char** nicknames = calloc(2, sizeof(char*));
  for(int i = 0; i < 2; i++) {
    if(clientes[i] == 0) {
      if((clientes[i] = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      printf("Aceptando conexion cliente %i. Esperando confirmacion\n", i);
      read(clientes[i], buffer, 2057);
      mensaje_recibir = decodificar(buffer);

      if(atoi(mensaje_recibir[0]) == start_conection) {  // es decir, si recibo id de start conection
        // Enviar mensaje de conection established
        printf("Recibo confirmacion. Envio respuesta\n");
        mensaje_enviar = codificar(conection_established, "");
        send(clientes[i], mensaje_enviar, strlen(mensaje_enviar), 0);
        free_codificacion(mensaje_enviar);
      } else {
        // quizas estos errores hay que manejarlos diferente
        perror("no se recibio mensaje de conexion establecida");
        exit(EXIT_FAILURE);
      }
      free_decodificacion(mensaje_recibir);

      printf("Solicito nickname\n");
      mensaje_enviar = codificar(ask_nickname, "");
      send(clientes[i], mensaje_enviar, strlen(mensaje_enviar), 0);
      free_codificacion(mensaje_enviar);

      read(clientes[i], buffer, 2057);
      mensaje_recibir = decodificar(buffer);
      if(atoi(mensaje_recibir[0]) != return_nickname) {  // es decir, si no recibo id de return nickname
        // quizas estos errores hay que manejarlos diferente
        perror("no se recibio mensaje de retornar nickname");
        exit(EXIT_FAILURE);
      }
      nicknames[i] = calloc(256, sizeof(char));
      strcpy(nicknames[i], mensaje_recibir[2]);
      free_decodificacion(mensaje_recibir);
    }
  }
  printf("tengo nickname %s y %s\n", nicknames[0], nicknames[1]);

  /* Envio oponentes */
  mensaje_enviar = codificar(opponent_found, nicknames[1]);
  send(clientes[0], mensaje_enviar, strlen(mensaje_enviar), 0);
  free_codificacion(mensaje_enviar);
  mensaje_enviar = codificar(opponent_found, nicknames[0]);
  send(clientes[1], mensaje_enviar, strlen(mensaje_enviar), 0);
  free_codificacion(mensaje_enviar);
  sleep(1);  // si no espero se acoplan mensajes

  int pot[2] = {1000, 1000};
  pot[0] = 1;
  printf("%i %i\n", pot[0], pot[1]);
  /* Envio pots */
  char* pot_string = malloc(sizeof(char*));
  itoa(pot[0], pot_string, 10);
  mensaje_enviar = codificar(initial_pot, pot_string);
  send(clientes[0], mensaje_enviar, strlen(mensaje_enviar), 0);
  free_codificacion(mensaje_enviar);
  itoa(pot[1], pot_string, 10);
  mensaje_enviar = codificar(initial_pot, pot_string);
  send(clientes[1], mensaje_enviar, strlen(mensaje_enviar), 0);
  free_codificacion(mensaje_enviar);
  free(pot_string);

  sleep(1);
  return 0;
}
