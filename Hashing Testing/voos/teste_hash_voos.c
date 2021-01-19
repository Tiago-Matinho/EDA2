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

int hashcode(char input[FLIGHT_CODE]){
	int key;

	key = POS(input[0]) * ALPHABET_SIZE * 9999;
	key += POS(input[1]) * 9999;

	if(input[3] == '\0'){
		key += NUM(input[2]);
		return key;
	}

	if(input[4] == '\0'){
		key += NUM(input[2]) * 10;
		key += NUM(input[3]);
		return key;
	}

	if(input[5] == '\0'){
		key += NUM(input[2]) * 100;
		key += NUM(input[3]) * 10;
		key += NUM(input[4]);
		return key;
	}

	key += NUM(input[2]) * 1000;
	key += NUM(input[3]) * 100;
	key += NUM(input[4]) * 10;
	key += NUM(input[5]);
	return key;
}


int main(){
	char input[FLIGHT_CODE];
	int key = -1;

	while(scanf("%s", input) != EOF){
		key = hashcode(input);

    	printf("%d\n", key);
	}

	return 0;
}