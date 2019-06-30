#include "air_hash.h"


struct vertice* vertice_new(char name[AIRPORT]){
    struct vertice* new = malloc(sizeof(struct vertice));

    if(new != NULL){
        memset(new->name, '\0', AIRPORT);
        strcpy(new->name, name);
        new->distance = INF;
        new->parent = NULL;
    }
    return new;
}


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
            new->vertices[i] = NULL;
        }
    }
    return new;
}


/*---------------------------------------------------------------------------*/


long int air_hashcode(char code[AIRPORT]){
    long int key;

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

    return key % MAX_AIR_HASH;
}


void air_insert(struct air_hash* hashtable, struct airport* air_new){
    long int key = air_hashcode(air_new->code);

    if(key < 0){
        printf("ERRO NO HASHING DE AEROPORTOS INSERT\n");
        exit(1);
    }

    key %= MAX_AIR_HASH;

    while(hashtable->table[key] != NULL){
        key++;
        key %= MAX_AIR_HASH;
    }

    //cria logo uma lista de voos de partida e vertice
    air_new->changed = true;
    hashtable->flights[key] = flight_list_new();
    hashtable->vertices[key] = vertice_new(air_new->code);
    hashtable->table[key] = air_new;
}


struct airport* air_search(struct air_hash* hashtable, char code[AIRPORT]){
    long int key = -1;
    key = air_hashcode(code);

    if(key < 0){
        printf("ERRO NO HASHING DE AEROPORTOS SEARCH\n");
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


long int air_search_index(struct air_hash* hashtable, char code[AIRPORT]){
    long int key = air_hashcode(code);

    if(key < 0){
        printf("ERRO NO HASHING DE AEROPORTOS SEARCH INDEX\n");
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
        printf("ERRO POINTER AIR_FP NULL. AIR_READ\n");
        exit(1);
    }

    struct airport* new_airport;

    fseek(air_fp, 0, SEEK_SET);

    for(int i = 0; i < MAX_AIRPORT; i++){
        new_airport = malloc(sizeof(struct airport));

        if(new_airport == NULL){
            printf("ERRO A CRIAR AEROPORTO. AIR_READ\n");
            exit(1);
        }

        memset(new_airport->code, '\0', AIRPORT);
        new_airport->changed = false;
        new_airport->pos = -1;
        new_airport->GMT = 0;

        fread(new_airport, sizeof(*new_airport), 1, air_fp);

        //ultimo aeroporto guardado tem sempre o code[0] = '\0'
        if(new_airport->code[0] == '\0'){
            free(new_airport);
            return;
        }

        air_insert(air_hash, new_airport);
    }
}


void air_write(FILE* air_fp, struct air_hash* air_hash){
    if(air_fp == NULL){
        printf("ERRO POINTER AIR_FP NULL. AIR_WRITE\n");
        exit(1);
    }

    fseek(air_fp, 0, SEEK_SET);

    struct airport* current;
    struct airport* blank = malloc(sizeof(struct airport));

    if(blank == NULL){
        printf("ERRO A CRIAR AEROPORTO. AIR_WRITE\n");
        exit(1);
    }

    //guarda todos os aeroportos presentes na hashtable
    for(int i = 0; i < MAX_AIR_HASH; i++){
        current = air_hash->table[i];

        if(current != NULL){
            if(current->changed)
                fwrite(current, sizeof(*current), 1, air_fp);

            free(current);
        }

        if(air_hash->flights[i] != NULL)
            flight_list_destroy(air_hash->flights[i]);

        if(air_hash->vertices[i] != NULL)
            free(air_hash->vertices[i]);
    }

    memset(blank->code, '\0', AIRPORT);
    blank->changed = false;
    blank->GMT = 0;
    blank->pos = -1;

    //guarda o ultimo aeroporto vazio, funciona como um delimitador
    fwrite(blank, sizeof(*blank), 1, air_fp);
    free(blank);
    free(air_hash);
}