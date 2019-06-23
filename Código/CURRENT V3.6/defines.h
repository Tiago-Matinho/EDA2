#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define FLIGHT_CODE 7
#define AIRPORT 5

#define TIME(H, M) (H * 60 + M)
//#define TO_GMT_H (S) (S / 60)
//#define TO_GMT_M (M) (M % 60)

#define MAX_AIRPORT 400003
#define MAX_FLIGHT  1500003
#define AIR_FILENAME "air_hash.bin"
#define FLY_FILENAME "flight_hash.bin"
#define FLY_LIST_FILENAME "fly_list.bin"

//Retirado da implementac~ao de Tries das aulas praticas
#define ALPHABET_MIN 'A'
#define ALPHABET_MAX 'Z'
#define ALPHABET_SIZE (ALPHABET_MAX - ALPHABET_MIN + 1)
#define POS(c)  ((c) - ALPHABET_MIN)
#define CHAR(n) ((n) + ALPHABET_MIN)
#define NUM(c) (c - '0')

