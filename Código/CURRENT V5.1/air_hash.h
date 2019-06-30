#include "flight_list.h"

struct vertice{
    char name[AIRPORT];     //nome do vertice
    struct flight* parent;  //voo pai
    int distance;
};

struct airport{
    char code[AIRPORT];
    int pos;                    //posic~ao do vertice do aeroporto na heap
    short GMT;                   //GMT 'e guardado em minutos
    bool changed;     
};

struct air_hash{
    struct airport* table[MAX_AIR_HASH];
    struct flight_list* flights[MAX_AIR_HASH];
    struct vertice* vertices[MAX_AIR_HASH];
};


/*---------------------------------------------------------------------------*/

struct vertice* vertice_new(char name[AIRPORT]);
struct airport* airport_new(char code[AIRPORT], short GMT);
struct air_hash* air_hash_new();


/*---------------------------------------------------------------------------*/


long int air_hashcode(char code[AIRPORT]);
void air_insert(struct air_hash* hashtable, struct airport* air_new);
struct airport* air_search(struct air_hash* hashtable, char code[AIRPORT]);
long int air_search_index(struct air_hash* hashtable, char code[AIRPORT]);


/*---------------------------------------------------------------------------*/


void air_read(FILE* air_fp, struct air_hash* air_hash);
void air_write(FILE* air_fp, struct air_hash* air_hash);