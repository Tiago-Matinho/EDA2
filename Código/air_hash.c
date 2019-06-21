#include "air_hash.h"


struct airport* airport_new(char name[AIRPORT], short GMT){
    struct airport* new_node = malloc(sizeof(struct airport));

    if(new_node != NULL){
        strcpy(new_node->name, name);
        new_node->GMT = GMT;
        new_node->fly_list = dep_list_new();
    }

    return new_node;
}


struct air_hash* air_hash_new(){
    struct air_hash* new_hashtable = malloc(sizeof(struct air_hash));

    if(new_hashtable != NULL){
        for(int i = 0; i < MAX_AIRPORT; i++)
            new_hashtable->table[i] = NULL;
    }

    return new_hashtable;
}


/*-------------------------------------------------------------------*/


int air_hashcode(char name[AIRPORT]){
    int key;

    //caso so' tenha 3 letras
    if(name[3] == '\0'){
        key = POS(name[0]) * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(name[1]) * ALPHABET_SIZE;
        key += POS(name[2]);
    }
    else{
        key = POS(name[0]) * ALPHABET_SIZE * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(name[1]) * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(name[2]) * ALPHABET_SIZE;
        key += POS(name[3]);
    }

    return key % MAX_AIRPORT;
}


void air_insert(struct air_hash* hashtable, struct airport* air_new){
    int key = air_hashcode(air_new->name);

    while(hashtable->table[key] != NULL){
        key++;
        key %= MAX_AIRPORT;
    }

    hashtable->table[key] = air_new;
}


int air_search(struct air_hash* hashtable, char name[AIRPORT]){
    int key = air_hashcode(name);

    while(hashtable->table[key] != NULL){
        if(strcmp(hashtable->table[key]->name, name) == 0)
            return key;

        key++;
        key %= MAX_AIRPORT;
    }

    return -1;
}


struct airport* air_get(struct air_hash* hashtable, char name[AIRPORT]){
    int key = air_search(hashtable, name);

    if(key == -1)
        return NULL;

    return hashtable->table[key];
}
