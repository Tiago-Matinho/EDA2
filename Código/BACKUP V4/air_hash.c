#include "air_hash.h"

struct airport* airport_new(char code[AIRPORT], short GMT){
    struct airport* new = malloc(sizeof(struct airport));

    if(new != NULL){
        memset(new->code, '\0', AIRPORT);
        strcpy(new->code, code);
        new->changed = true;
        new->GMT = GMT;
        new->pos = -1;
    }
    return new;
}


struct air_hash* air_hash_new(){
    struct air_hash* new = malloc(sizeof(struct air_hash));

    if(new != NULL){
        for(int i = 0; i < MAX_AIR_HASH; i++){
            new->table[i] = NULL;
            new->flights[i] = NULL;
        }
        return new;
    }
}


/*---------------------------------------------------------------------------*/


int air_hashcode(char code[AIRPORT]){
    int key;

    //caso so' tenha 3 letras
    if(code[3] == '\0'){
        key = POS(code[0]) * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(code[1]) * ALPHABET_SIZE;
        key += POS(code[2]);
    }
    else{
        key = POS(code[0] + 1) * ALPHABET_SIZE * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(code[1]) * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(code[2]) * ALPHABET_SIZE;
        key += POS(code[3]);
    }

    return key;
}


void air_insert(struct air_hash* hashtable, struct airport* air_new){
    int key = air_hashcode(air_new->code);

    if(key < 0){
        printf("ERRO NO HASHING DE AEROPORTOS\n");
        exit(1);
    }

    key %= MAX_AIR_HASH;

    while(hashtable->table[key] != NULL){
        key++;
        key %= MAX_AIR_HASH;
    }

    air_new->changed = true;
    hashtable->flights[key] = flight_list_new();
    hashtable->table[key] = air_new;
}


struct airport* air_search(struct air_hash* hashtable, char code[AIRPORT]){
    int key = air_hashcode(code);

    if(key < 0){
        printf("ERRO NO HASHING DE AEROPORTOS\n");
        exit(1);
    }

    key %= MAX_AIR_HASH;

    while(hashtable->table[key] != NULL){
        if(strcmp(hashtable->table[key]->code, code) == 0)
            return hashtable->table[key];

        key++;
        key %= MAX_AIR_HASH;
    }
    return NULL;
}


int air_search_index(struct air_hash* hashtable, char code[AIRPORT]){
    int key = air_hashcode(code);

    if(key < 0){
        printf("ERRO NO HASHING DE AEROPORTOS\n");
        exit(1);
    }

    key %= MAX_AIR_HASH;

    while(hashtable->table[key] != NULL){
        if(strcmp(hashtable->table[key]->code, code) == 0)
            return key;

        key++;
        key %= MAX_AIR_HASH;
    }
    return -1;
}


/*---------------------------------------------------------------------------*/


void air_read(FILE* air_fp, struct air_hash* air_hash){
    if(air_fp == NULL){
        printf("ERRO COM FICHEIRO AIR_READ\n");
        exit(1);
    }

    struct airport* new_airport;

    fseek(air_fp, 0, SEEK_SET);

    for(int i = 0; i < MAX_AIRPORT; i++){
        new_airport = malloc(sizeof(struct airport));

        if(new_airport == NULL){
            printf("ERRO A CRIAR AEROPORTO\n");
            exit(1);
        }

        memset(new_airport->code, '\0', AIRPORT);
        new_airport->changed = false;
        new_airport->pos = -1;
        new_airport->GMT = 0;

        fread(new_airport, sizeof(*new_airport), 1, air_fp);

        if(new_airport->code[0] == '\0'){
            free(new_airport);
            return;
        }

        air_insert(air_hash, new_airport);
    }


}


void air_write(FILE* air_fp, struct air_hash* air_hash, bool first_time){
    if(air_fp == NULL){
        printf("ERRO COM FICHEIRO AIR_READ\n");
        exit(1);
    }

    fseek(air_fp, 0, SEEK_SET);

    struct airport* current;
    struct airport* blank = malloc(sizeof(struct airport));

    if(blank == NULL){
        printf("ERRO A CRIAR AEROPORTO\n");
        exit(1);
    }

    for(int i = 0; i < MAX_AIR_HASH; i++){
        current = air_hash->table[i];

        if(current != NULL){
            if(current->changed)
                fwrite(current, sizeof(*current), 1, air_fp);

            flight_list_destroy(air_hash->flights[i]);
            free(current);
        }
    }

    memset(blank->code, '\0', AIRPORT);
    blank->changed = false;
    blank->GMT = 0;
    blank->pos = -1;

    fwrite(blank, sizeof(*blank), 1, air_fp);
    free(blank);
    free(air_hash);
}