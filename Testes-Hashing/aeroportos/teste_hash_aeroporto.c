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

    //caso so' tenha 3 letras
    if(input[3] == '\0'){
        key = POS(input[0]) * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(input[1]) * ALPHABET_SIZE;
        key += POS(input[2]);
    }
    else{
        key = POS(input[0] + 1) * ALPHABET_SIZE * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(input[1]) * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(input[2]) * ALPHABET_SIZE;
        key += POS(input[3]);
    }

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