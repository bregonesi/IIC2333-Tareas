#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "funciones.h"

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
	char* final = malloc(sizeof(char) * cantidad);
	for(int i = 0; i < cantidad - (int)strlen(binario); i++) strcat(final, "0");
	strcat(final, binario);

	return final;
}

char* cut_string(char* string, int inicio, int final) {
	char* string_final = malloc(sizeof(char) * 20);
	for(int i = 0; i < (final - inicio); i++) {
		string_final[i] = string[inicio + i];
	}
	string_final[final - inicio] = '\0';

	return string_final;
}
