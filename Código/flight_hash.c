#include "flight_hash.h"


struct flight* flight_new(char name[FLIGHT_CODE], char air_dep[AIRPORT], char air_dest[AIRPORT], short dep_time, short duration){
    struct flight* new_node = malloc(sizeof(struct flight));

    if(new_node != NULL){
        strcpy(new_node->name, name);
        strcpy(new_node->air_dep, air_dep);
        strcpy(new_node->air_dest, air_dest);
        new_node->dep_time = dep_time;
        new_node->duration = duration;
        new_node->erased = false;
    }

    return new_node;
}


struct fly_hash* fly_hash_new(){
    struct fly_hash* new_hashtable = malloc(sizeof(struct fly_hash));

    if(new_hashtable != NULL){
        for(int i = 0; i < MAX_FLIGHT; ++i)
            new_hashtable->table[i] = NULL;
    }

    return new_hashtable;
}


int fly_hashcode(char name[FLIGHT_CODE]){
    int key;

    key = POS(name[0])  * ALPHABET_SIZE* 10000;
    key += POS(name[1]) * 10000;
    key += 1000 * NUM(name[2]);
    key += 100 * NUM(name[3]);
    key += 10 * NUM(name[4]);
    key += NUM(name[5]);

    return key % MAX_FLIGHT;
}


void fly_insert(struct fly_hash* hashtable, struct flight* new_flight){
    int key = fly_hashcode(new_flight->name);

    while(hashtable->table[key] != NULL){
        key++;
        key %= MAX_FLIGHT;
    }

    hashtable->table[key] = new_flight;
}


int fly_search(struct fly_hash* hashtable, char name[FLIGHT_CODE]){
    int key = fly_hashcode(name);

    while(hashtable->table[key] != NULL){
        if(strcmp(hashtable->table[key]->name, name) == 0)
            return key;

        key++;
        key %= MAX_FLIGHT;
    }

    return -1;
}


struct flight* fly_get(struct fly_hash* hashtable, char name[FLIGHT_CODE]){
    int key = fly_search(hashtable, name);

    if(key == -1)
        return NULL;

    if(hashtable->table[key]->erased)
        return NULL;

    return hashtable->table[key];
}