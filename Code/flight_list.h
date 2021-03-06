#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Retirado da implementac~ao de Tries das aulas praticas
#define ALPHABET_MIN 'A'
#define ALPHABET_MAX 'Z'
#define ALPHABET_SIZE (ALPHABET_MAX - ALPHABET_MIN + 1)
#define POS(c)  ((c) - ALPHABET_MIN)
#define NUM(c) ((c) - '0')

#define COMMAND_MAX 3
#define AIRPORT 5
#define MAX_AIRPORT 200000
#define MAX_AIR_HASH (MAX_AIRPORT + (MAX_AIRPORT) + 3)
#define AIR_FILENAME "air_hash.bin"

#define FLIGHT_CODE 7
#define MAX_FLIGHT 750000
#define MAX_FLIGHT_HASH (MAX_FLIGHT + (MAX_FLIGHT) + 3)
#define FLY_FILENAME "flight_hash.bin"

#define INF 2147483647
#define HEAP_SIZE (MAX_AIRPORT + 2)
#define PARENT(i) ((i) / 2)
#define LEFT(i) (2 * (i))
#define RIGHT(i) (2 * (i) + 1)

#define CONNECTION_TIME 30
#define MIN_IN_DAY (24 * 60)
#define MIN_IN_H 60
#define TIME(h, m) ((h) * MIN_IN_H + (m))
#define TO_GMT_H(m) ((m) / MIN_IN_H)
#define TO_GMT_M(m) ((m) % MIN_IN_H)


struct flight{
    char name[FLIGHT_CODE];
    char dep[AIRPORT];
    char dest[AIRPORT];
    short dep_time;
    short duration;
};


struct list_node{
    struct flight* element;
    struct list_node* next;
};


struct flight_list{
    struct list_node* header;
};


/*---------------------------------------------------------------------------*/


struct list_node* list_node_new(struct flight* flight);
struct flight_list* flight_list_new();


/*-------------------------------------------------------------------*/


bool flight_list_empty(struct flight_list* list);
void flight_list_insert(struct flight_list* list, struct list_node* new_node);
void flight_list_remove(struct flight_list* list, char name[FLIGHT_CODE]);
struct list_node* flight_list_pop(struct flight_list* list);


/*---------------------------------------------------------------------------*/


void flight_list_destroy(struct flight_list* list);