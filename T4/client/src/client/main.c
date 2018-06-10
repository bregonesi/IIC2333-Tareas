#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

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
  Decodificar_Mazo* mensaje_recibir_cartas;  // estructura

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

      /* Initial bet */
      read(sock, buffer, 2057);
      mensaje_recibir = decodificar(buffer);
      if(atoi(mensaje_recibir[0]) != initial_bet) {
        // quizas estos errores hay que manejarlos diferente
        perror("no se recibio initial bet");
        exit(EXIT_FAILURE);
      }
      printf("%s\n", mensajes[atoi(mensaje_recibir[0])]);
      free_decodificacion(mensaje_recibir);

      /* Recibo cartas originales */
      read(sock, buffer, 2057);
      mensaje_recibir_cartas = decodificar_cartas(buffer);
      if(atoi(mensaje_recibir_cartas->mensaje_id) != five_cards) {
        // quizas estos errores hay que manejarlos diferente
        perror("no se recibio cinco cartas");
        exit(EXIT_FAILURE);
      }

      /* whos first */
      read(sock, buffer, 2057);
      mensaje_recibir = decodificar(buffer);
      if(atoi(mensaje_recibir[0]) != whos_first) {
        // quizas estos errores hay que manejarlos diferente
        perror("no se recibio whos first");
        exit(EXIT_FAILURE);
      }

      if(mensaje_recibir[2][0] == 1)
        printf("Tu juegas\n");
      else {
        printf("Parte el otro. Esperas\n");
      }
      free_decodificacion(mensaje_recibir);

      /* Espero mi turno */
      read(sock, buffer, 2057);
      mensaje_recibir = decodificar(buffer);
      if(atoi(mensaje_recibir[0]) != get_cards_to_cange) {
        // quizas estos errores hay que manejarlos diferente
        perror("no se recibio get cards to change");
        exit(EXIT_FAILURE);
      }

      /* Printeo opciones de cartas a eliminar */
      printf("%s\n", mensajes[atoi(mensaje_recibir_cartas->mensaje_id)]);
      for(int k = 0; k < mensaje_recibir_cartas->cantidad_cartas; k++) {
        printf("[%i]: %i %i\n", k + 1, mensaje_recibir_cartas->cartas[k][0], mensaje_recibir_cartas->cartas[k][1]);
      }
      printf("Escoja numeros de carta a eliminar ('end' para terminar):\n");
      int** cartas_eliminar = malloc(5 * sizeof(int*));
      int cantidad_cartas_eliminar = 0;
      char input_text[4];
      while(fgets(input_text, 4, stdin)) {
        //input_text[strlen(input_text) - 1] = '\0';  // sino queda con \n el final

        if(atoi(input_text) > 0 && atoi(input_text) <= 5) {
          cartas_eliminar[cantidad_cartas_eliminar] = mensaje_recibir_cartas->cartas[atoi(input_text) - 1];
          cantidad_cartas_eliminar++;
        }

        if(strcmp(input_text, "end") == 0)
          break;
      }

      /* Envio cartas a elimiar */
      mensaje_enviar = codificar_cartas(return_cards_to_change, cartas_eliminar, cantidad_cartas_eliminar);
      free_decodificacion_cartas(mensaje_recibir_cartas);
      send(sock, mensaje_enviar, strlen(mensaje_enviar), 0);
      free_codificacion(mensaje_enviar);

      /* Cartas definitivas */
      read(sock, buffer, 2057);
      mensaje_recibir_cartas = decodificar_cartas(buffer);
      if(atoi(mensaje_recibir_cartas->mensaje_id) != five_cards) {
        // quizas estos errores hay que manejarlos diferente
        perror("no se recibio cinco cartas");
        exit(EXIT_FAILURE);
      }
      printf("%s\n", mensajes[atoi(mensaje_recibir_cartas->mensaje_id)]);
      for(int k = 0; k < mensaje_recibir_cartas->cantidad_cartas; k++) {
        printf("[%i]: %i %i\n", k + 1, mensaje_recibir_cartas->cartas[k][0], mensaje_recibir_cartas->cartas[k][1]);
      }

      read(sock, buffer, 2057);
      mensaje_recibir = decodificar(buffer);
      printf("%s\n", buffer);
      if(atoi(mensaje_recibir[0]) != get_bet) {
        // quizas estos errores hay que manejarlos diferente
        perror("no se recibio get bet");
        exit(EXIT_FAILURE);
      }
      printf("Bets (id) disponibles:\n");
      for(int k = 0; k < atoi(mensaje_recibir[1]); k++) {
        printf("%i\n", mensaje_recibir[2][k]);
      }
      int bet = 1;
      while(bet == 1) {
        /* Espero los bets */
        printf("Escoja un bet\n");
        char input_text[2];
        while(1) {
          fgets(input_text, 2, stdin);
          if(strcmp(input_text, "") != 0 && strcmp(input_text, "\n") != 0)
            break;

          /*
          if(strcmp(input_text, "") != 0 && atoi(input_text) >= mensaje_recibir[2][0] && atoi(input_text) <= mensaje_recibir[2][atoi(mensaje_recibir[1]) - 1]) {
            break;
          }
          */
          //printf("Escogiste un bet malo\n");
        }
        printf("Escogiste el bet %s\n", input_text);

        /* Envio bet */
        int* bets = malloc(sizeof(int));
        bets[0] = atoi(input_text);
        mensaje_enviar = codificar_ints(return_bet, bets, 1);
        printf("%s\n", mensaje_enviar);
        send(sock, mensaje_enviar, strlen(mensaje_enviar), 0);
        free_codificacion(mensaje_enviar);

        /* Espero respuesta de bet */
        read(sock, buffer, 2057);
        mensaje_recibir = decodificar(buffer);
        if(atoi(mensaje_recibir[0]) != error_bet && atoi(mensaje_recibir[0]) != ok_bet) {
          // quizas estos errores hay que manejarlos diferente
          perror("no se recibio ni error bet ni ok bet");
          exit(EXIT_FAILURE);
        }

        if(atoi(mensaje_recibir[0]) == ok_bet) {
          printf("Se recibio bet bueno. Se finaliza programa ya que no se alcanzo a desarrollar\n");
          return 0;
          break;
        }
      }

    }
  }

  return 0;
}
