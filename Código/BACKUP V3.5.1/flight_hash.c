#include "flight_hash.h"


struct flight* flight_new(char name[FLIGHT_CODE], char dep[AIRPORT], char dest[AIRPORT], short dep_time, short duration){
    struct flight* new_node = malloc(sizeof(struct flight));

    if(new_node != NULL){
        memset(new_node->name, '\0', FLIGHT_CODE);
        memset(new_node->dep, '\0', AIRPORT);
        memset(new_node->dest, '\0', AIRPORT);

        strcpy(new_node->name, name);
        strcpy(new_node->dep, dep);
        strcpy(new_node->dest, dest);
        new_node->dep_time = dep_time;
        new_node->duration = duration;
        new_node->erased = false;
        new_node->changed = true;
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

/*-------------------------------------------------------------------*/

int fly_hashcode(char name[FLIGHT_CODE]){
    int key;

    key = POS(name[0]) * ALPHABET_SIZE * 9999;
    key += POS(name[1]) * 9999;

    if(name[3] == '\0'){
        key += NUM(name[2]);
        return key;
    }

    if(name[4] == '\0'){
        key += NUM(name[2]) * 10;
        key += NUM(name[3]);
        return key;
    }

    if(name[5] == '\0'){
        key += NUM(name[2]) * 100;
        key += NUM(name[3]) * 10;
        key += NUM(name[4]);
        return key;
    }

    key += NUM(name[2]) * 1000;
    key += NUM(name[3]) * 100;
    key += NUM(name[4]) * 10;
    key += NUM(name[5]);
    return key;
}


void fly_insert(struct fly_hash* hashtable, struct flight* new_flight){
    int key = fly_hashcode(new_flight->name);
    key %= MAX_FLIGHT;

    while(hashtable->table[key] != NULL){
        key++;
        key %= MAX_FLIGHT;
    }

    hashtable->table[key] = new_flight;
}


int fly_search(struct fly_hash* hashtable, char name[FLIGHT_CODE]){
    int key = fly_hashcode(name) % MAX_FLIGHT;

    while(hashtable->table[key] != NULL){
        if(strcmp(hashtable->table[key]->name, name) == 0 && !hashtable->table[key]->erased)
            return key;

        key++;
        key %= MAX_FLIGHT;
    }

    return -1;
}


struct flight* fly_get(struct fly_hash* hashtable, char name[FLIGHT_CODE]){
    int key = fly_search(hashtable, name) % MAX_FLIGHT;

    if(key == -1)
        return NULL;

    if(hashtable->table[key]->erased)
        return NULL;

    return hashtable->table[key];
}

/*-------------------------------------------------------------------*/

void fly_read(FILE* fly_file, struct fly_hash* fly_hash, char flight[FLIGHT_CODE]){
    if(fly_file == NULL){
        printf("ERRO A LER FLY_HASH\n");
        exit(1);
    }

    int key = fly_hashcode(flight);
    fseek(fly_file, key * sizeof(struct flight), SEEK_SET);

    struct flight* new_flight = malloc(sizeof(struct flight));

    fread(new_flight, sizeof(*new_flight), 1, fly_file);

    if(strcmp(flight, new_flight->name) != 0 || new_flight->erased){
        free(new_flight);
        return;
    }

    fly_insert(fly_hash, new_flight);
}


void fly_write(FILE* fly_file, struct fly_hash* fly_hash){
    if(fly_file == NULL){
        printf("ERRO A ESCREVER FLY_HASH\n");
        exit(1);
    }

    int key;
    struct flight* current_flight;

    for(int i = 0; i < MAX_FLIGHT; ++i){
        current_flight = fly_hash->table[i];

        if(current_flight != NULL){

            if(current_flight->changed){
                current_flight->changed = false;

                key = fly_hashcode(current_flight->name);

                fseek(fly_file, key * sizeof(struct flight), SEEK_SET);
                fwrite(current_flight, sizeof(*current_flight), 1, fly_file);
            }
            free(current_flight);
        }
    }
}