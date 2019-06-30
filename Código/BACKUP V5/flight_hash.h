#include "air_hash.h"

struct fly_hash{
    struct flight* table[MAX_FLIGHT_HASH];
};


/*---------------------------------------------------------------------------*/


struct flight* flight_new(char name[FLIGHT_CODE], char dep[AIRPORT],
                          char dest[AIRPORT], short dep_time, short duration);
struct fly_hash* fly_hash_new();


/*---------------------------------------------------------------------------*/


long int fly_hashcode(char name[FLIGHT_CODE]);
void fly_insert(struct fly_hash* hashtable, struct flight* new_flight);
struct flight* fly_search(struct fly_hash* hashtable, char name[FLIGHT_CODE]);
long int fly_search_index(struct fly_hash* hashtable, char name[FLIGHT_CODE]);



/*---------------------------------------------------------------------------*/


void fly_read(FILE* fly_fp, struct fly_hash* hashtable, struct air_hash* air_hash);
void fly_write(FILE* fly_fp, struct fly_hash* hashtable);