#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define FLIGHT_CODE 7
#define AIRPORT 5

#define TIME(H, M) H * 60 + M
#define TO_GMT_H (M) M / 60
#define TO_GMT_M (M) M % 60

#define MAX_AIRPORT 200003
#define MAX_FLIGHT 750003

//Retirado da implementac~ao de Tries das aulas praticas
#define ALPHABET_MIN 'A'
#define ALPHABET_MAX 'Z'
#define ALPHABET_SIZE (ALPHABET_MAX - ALPHABET_MIN + 1)
#define POS(c)  ((c) - ALPHABET_MIN)
#define CHAR(n) ((n) + ALPHABET_MIN)
#define NUM(c) (c - '0')

#define letras 26*26

int main(){
	char input[FLIGHT_CODE];
	int key = 0;

	for(int i = 0; i < 26*26*26*26; i++)
		printf("%d\n", i);

	return 0;
}