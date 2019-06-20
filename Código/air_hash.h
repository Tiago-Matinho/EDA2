#include "flight_hash.h"

struct airport{
    char name[AIRPORT];
    short GMT;
    struct dep_list* fly_list;
};

struct air_hash{
    struct airport* table[MAX_AIRPORT];
};

/*-------------------------------------------------------------------*/

struct airport* airport_new(char name[AIRPORT], short GMT);
struct air_hash* air_hash_new();

int air_hashcode(char name[AIRPORT]);
void air_insert(struct air_hash* hashtable, struct airport* air_new);
int air_search(struct air_hash* hashtable, char name[AIRPORT]);
struct airport* air_get(struct air_hash* hashtable, char name[AIRPORT]);

void air_read();
void air_write();
void air_destroy();