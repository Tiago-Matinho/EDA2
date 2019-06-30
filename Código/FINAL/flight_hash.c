#include "flight_hash.h"


struct flight* flight_new(char name[FLIGHT_CODE], char dep[AIRPORT],
                          char dest[AIRPORT], short dep_time, short duration){

    struct flight* new = malloc(sizeof(struct flight));

    if(new != NULL){
        memset(new->name, '\0', FLIGHT_CODE);
        memset(new->dep, '\0', AIRPORT);
        memset(new->dest, '\0', AIRPORT);

        strcpy(new->name, name);
        strcpy(new->dep, dep);
        strcpy(new->dest, dest);
        new->dep_time = dep_time;
        new->duration = duration;
    }
    return new;
}


struct fly_hash* fly_hash_new(){
    struct fly_hash* new_hashtable = malloc(sizeof(struct fly_hash));

    if(new_hashtable != NULL){
        for(int i = 0; i < MAX_FLIGHT_HASH; i++)
            new_hashtable->table[i] = NULL;
    }

    return new_hashtable;
}


/*---------------------------------------------------------------------------*/


long int fly_hashcode(char name[FLIGHT_CODE]){
    long int key;

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
    return key % MAX_FLIGHT_HASH;
}


void fly_insert(struct fly_hash* hashtable, struct flight* new_flight){
    long int key = fly_hashcode(new_flight->name);

    if(key < 0){
        printf("ERRO NO HASHING DE VOOS\n");
        exit(1);
    }

    key %= MAX_FLIGHT_HASH;

    while(hashtable->table[key] != NULL){
        key++;
        key %= MAX_FLIGHT_HASH;
    }

    hashtable->table[key] = new_flight;
}


struct flight* fly_search(struct fly_hash* hashtable, char name[FLIGHT_CODE]){
    long int key = fly_hashcode(name);

    if(key < 0){
        printf("ERRO NO HASHING DE VOOS\n");
        exit(1);
    }

    key %= MAX_FLIGHT_HASH;

    while(hashtable->table[key] != NULL){
        if(strcmp(hashtable->table[key]->name, name) == 0)
            return hashtable->table[key];

        key++;
        key %= MAX_FLIGHT_HASH;
    }

    return NULL;
}

long int fly_search_index(struct fly_hash* hashtable, char name[FLIGHT_CODE]){
    long int key = fly_hashcode(name);

    if(key < 0){
        printf("ERRO NO HASHING DE VOOS\n");
        exit(1);
    }

    key %= MAX_FLIGHT_HASH;

    while(hashtable->table[key] != NULL){
        if(strcmp(hashtable->table[key]->name, name) == 0)
            return key;

        key++;
        key %= MAX_FLIGHT_HASH;
    }

    return -1;
}

/*---------------------------------------------------------------------------*/


void fly_read(FILE* fly_fp, struct fly_hash* hashtable, struct air_hash* air_hash){

    if(fly_fp == NULL){
        printf("ERRO A LER FLY_HASH\n");
        exit(1);
    }

    struct list_node* node;
    struct flight* new_flight;
    long int key;
    fseek(fly_fp, 0, SEEK_SET);

    for(int i = 0; i < MAX_FLIGHT; ++i){
        new_flight = malloc(sizeof(struct flight));

        if(new_flight == NULL){
            printf("ERRO A CRIAR NOVO FLIGHT\n");
            exit(1);
        }

        memset(new_flight->name, '\0', FLIGHT_CODE);
        memset(new_flight->dep, '\0', AIRPORT);
        memset(new_flight->dest, '\0', AIRPORT);


        fread(new_flight, sizeof(*new_flight), 1, fly_fp);

        if(new_flight->name[0] == '\0'){
            free(new_flight);
            return;
        }

        key = air_search_index(air_hash, new_flight->dep);

        if(key < 0){
            printf("ERRO NO HASHING DE AEROPORTOS\n");
            exit(1);
        }

        node = list_node_new(new_flight);
        flight_list_insert(air_hash->flights[key], node);
        fly_insert(hashtable, new_flight);
    }
}


void fly_write(FILE* fly_fp, struct fly_hash* hashtable){
    if(fly_fp == NULL){
        printf("ERRO A ESCREVER FLY_HASH\n");
        exit(1);
    }

    struct flight* current;
    struct flight* blank = malloc(sizeof(struct flight));

    if(blank == NULL){
        printf("ERRO A CRIAR VOO\n");
        exit(1);
    }

    memset(blank->name, '\0', FLIGHT_CODE);
    memset(blank->dep, '\0', AIRPORT);
    memset(blank->dest, '\0', AIRPORT);
    blank->duration = 0;
    blank->dep_time = 0;


    fseek(fly_fp, 0, SEEK_SET);

    for(int i = 0; i < MAX_FLIGHT_HASH; ++i){
        current = hashtable->table[i];

        if(current != NULL){
            fwrite(current, sizeof(*current), 1, fly_fp);
            free(current);
        }
    }

    fwrite(blank, sizeof(*blank), 1, fly_fp);

    free(blank);
    free(hashtable);
    fclose(fly_fp);
}