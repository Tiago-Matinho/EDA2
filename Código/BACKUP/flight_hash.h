#include "dep_list.h"

struct flight{
    char name[FLIGHT_CODE];
    char air_dep[AIRPORT];
    char air_dest[AIRPORT];
    short dep_time;
    short duration;
    bool erased;
};

struct fly_hash{
    struct flight* table[MAX_FLIGHT];
};

/*-------------------------------------------------------------------*/

struct flight* flight_new(char name[FLIGHT_CODE], char air_dep[AIRPORT], char air_dest[AIRPORT], short dep_time, short duration);
struct fly_hash* fly_hash_new();

int fly_hashcode(char name[FLIGHT_CODE]);
void fly_insert(struct fly_hash* hashtable, struct flight* new_flight);
int fly_search(struct fly_hash* hashtable, char name[FLIGHT_CODE]);
struct flight* fly_get(struct fly_hash* hashtable, char name[FLIGHT_CODE]);

void fly_read();
void fly_write();
void fly_destroy();