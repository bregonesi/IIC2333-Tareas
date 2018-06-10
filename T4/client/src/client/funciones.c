#include "funciones.h"

/* Net code and decode */
char* codificar(int mensaje_id, char* mensaje) {
	char* retornar = calloc(2057, sizeof(char));  // 8 + 8 + 8*255 + 1

	char* mensaje_id_binario = calloc(9, sizeof(char));
	itoa(mensaje_id, mensaje_id_binario, 2);
	mensaje_id_binario = fill_binario(mensaje_id_binario, 8);

	char* payload_size_binario = calloc(9, sizeof(char));
	itoa((int)strlen(mensaje), payload_size_binario, 2);
	payload_size_binario = fill_binario(payload_size_binario, 8);

	char* payload = calloc(2041, sizeof(char));
	for(int i = 0; i < strlen(mensaje); i++) {
		char* caracter = calloc(9, sizeof(char));
		itoa(mensaje[i], caracter, 2);
		caracter = fill_binario(caracter, 8);
		strcat(payload, caracter);
		free(caracter);
	}

	strcat(retornar, mensaje_id_binario);
	free(mensaje_id_binario);
	strcat(retornar, payload_size_binario);
	free(payload_size_binario);
	strcat(retornar, payload);
	free(payload);

	return retornar;
}

char* codificar_mazo(int mensaje_id, int** cartas, int cantidad_cartas) {
	char* retornar = calloc(2057, sizeof(char));  // 8 + 8 + 8*255 + 1

	char* mensaje_id_binario = calloc(9, sizeof(char));
	itoa(mensaje_id, mensaje_id_binario, 2);
	mensaje_id_binario = fill_binario(mensaje_id_binario, 8);

	char* payload_size_binario = calloc(9, sizeof(char));
	itoa(cantidad_cartas*2, payload_size_binario, 2);
	payload_size_binario = fill_binario(payload_size_binario, 8);

	char* payload = calloc(2041, sizeof(char));
	for(int i = 0; i < cantidad_cartas; i++) {
		for (int j = 0; j < 2; j++) {
			char* caracter = calloc(9, sizeof(char));
			itoa(cartas[i][j], caracter, 2);
			caracter = fill_binario(caracter, 8);
			strcat(payload, caracter);
			free(caracter);
		}
	}

	strcat(retornar, mensaje_id_binario);
	free(mensaje_id_binario);
	strcat(retornar, payload_size_binario);
	free(payload_size_binario);
	strcat(retornar, payload);
	free(payload);

	return retornar;
}

char** decodificar(char* codificado) {
	char* mensaje_id_binario = calloc(9, sizeof(char));
	char* payload_size_binario = calloc(9, sizeof(char));
	char* mensaje_binario = calloc(2041, sizeof(char));

	for(int i = 0; i < strlen(codificado); i++)  // like python :D (sin {})
		if(i >= 0 && i < 8)
			mensaje_id_binario[i] = codificado[i];
		else if(i >= 8 && i < 16)
			payload_size_binario[i - 8] = codificado[i];
	 	else
			mensaje_binario[i - 16] = codificado[i];

	int mensaje_id = bin_to_dec(mensaje_id_binario);
	free(mensaje_id_binario);
	int payload_size = bin_to_dec(payload_size_binario);
	free(payload_size_binario);

	char* mensaje = calloc(256, sizeof(char));
	for(int i = 0; i < payload_size; i++) {
		char* caracter_binario = calloc(9, sizeof(char));
		printf("%s\n", caracter_binario);
		for(int j = 0; j < 8; j++)
			caracter_binario[j] = mensaje_binario[i * 8 + j];
		int caracter = bin_to_dec(caracter_binario);
		free(caracter_binario);
		mensaje[i] = caracter;
	}

	char* mensaje_id_char = calloc(2, sizeof(char));
	itoa(mensaje_id, mensaje_id_char, 10);
	char* payload_size_char = calloc(2, sizeof(char));
	itoa(payload_size, payload_size_char, 10);

	char** retornar = calloc(3, sizeof(char*));
	retornar[0] = mensaje_id_char;
	retornar[1] = payload_size_char;
	retornar[2] = mensaje;

	return retornar;
}

void free_codificacion(char* codificado) {
	free(codificado);
}

void free_decodificacion(char** decodificado) {
	free(decodificado[0]);
	free(decodificado[1]);
	free(decodificado[2]);
	free(decodificado);
}

/* Manejo de numeros */
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

char* fill_binario(char* binario, int cantidad) {
	char* final = calloc(cantidad + 1, sizeof(char));  // calloc por que hay q inicializar

	for(int i = 0; i < cantidad - (int)strlen(binario); i++) strcat(final, "0");
	strcat(final, binario);

	return final;
}

int bin_to_dec(char* bin) {
	int dec = 0;
	char c;

	for(int i = 0; i < strlen(bin); i++) {
		c = bin[i];
		if(c == '1') dec = dec * 2 + 1;
		else if(c == '0') dec *= 2;
	}

	return dec;
}
