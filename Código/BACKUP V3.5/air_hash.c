#include "air_hash.h"


struct airport* airport_new(char name[AIRPORT], short GMT){
    struct airport* new_node = malloc(sizeof(struct airport));

    if(new_node != NULL){
        memset(new_node->name, '\0', AIRPORT);
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
        key = POS(name[0] + 1) * ALPHABET_SIZE * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(name[1]) * ALPHABET_SIZE * ALPHABET_SIZE;
        key += POS(name[2]) * ALPHABET_SIZE;
        key += POS(name[3]);
    }

    return key;
}


void air_insert(struct air_hash* hashtable, struct airport* air_new){
    int key = air_hashcode(air_new->name) % MAX_AIRPORT;

    while(hashtable->table[key] != NULL){
        key++;
        key %= MAX_AIRPORT;
    }

    hashtable->table[key] = air_new;
}


int air_search(struct air_hash* hashtable, char name[AIRPORT]){
    int key = air_hashcode(name) % MAX_AIRPORT;

    while(hashtable->table[key] != NULL){
        if(strcmp(hashtable->table[key]->name, name) == 0)
            return key;

        key++;
        key %= MAX_AIRPORT;
    }

    return -1;
}


struct airport* air_get(struct air_hash* hashtable, char name[AIRPORT]){
    int key = air_search(hashtable, name) % MAX_AIRPORT;

    if(key == -1)
        return NULL;

    return hashtable->table[key];
}

/*-------------------------------------------------------------------*/

void air_read(FILE* air_file, FILE* flight_list_file, FILE* fly_file, struct fly_hash* fly_hash,
              struct air_hash* air_hash, char airport[AIRPORT]){
    if(air_file == NULL){
        printf("ERRO A LER AIR_HASH\n");
        exit(1);
    }

    int key = air_hashcode(airport);
    fseek(air_file, key * sizeof(struct airport), SEEK_SET);

    struct airport* new_airport = malloc(sizeof(struct airport));

    fread(new_airport, sizeof(*new_airport), 1, air_file);

    if(strcmp(new_airport->name, airport) != 0){
        free(new_airport);
        return;
    }

    new_airport->fly_list = dep_list_new();
    dep_list_read(flight_list_file, fly_file, fly_hash, new_airport->fly_list, key);

    air_insert(air_hash, new_airport);
}

void air_write(FILE* air_file, FILE* flight_list_file, struct air_hash* air_hash){
    if(air_file == NULL){
        printf("ERRO A ESCREVER AIR_HASH\n");
        exit(1);
    }

    struct airport* current_airport;
    int key;

    for(int i = 0; i < MAX_AIRPORT; ++i){
        current_airport = air_hash->table[i];

        if(current_airport != NULL){
            key = air_hashcode(current_airport->name);

            dep_list_write(flight_list_file, current_airport->fly_list, key);

            fseek(air_file, key * sizeof(struct airport), SEEK_SET);
            fwrite(current_airport, sizeof(*current_airport), 1, air_file);
            free(current_airport);
        }
    }
}