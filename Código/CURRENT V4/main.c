#include "min_heap.h"


FILE* open_air_file(FILE* air_fp, bool* air_first_time){
    air_fp = fopen(AIR_FILENAME, "r+");

    if(air_fp == NULL){
        air_fp = fopen(AIR_FILENAME, "w+");

        if(air_fp == NULL){
            printf("ERRO A CRIAR FICHEIRO ESCRITA. AIR_HASH.\n");
            exit(1);
        }

        *air_first_time = true;
    }
    return air_fp;
}


FILE* open_fly_file(FILE* fly_fp, bool* fly_first_time){
    fly_fp = fopen(FLY_FILENAME, "r+");

    if(fly_fp == NULL){
        fly_fp = fopen(FLY_FILENAME, "w+");

        if(fly_fp == NULL){
            printf("ERRO A CRIAR FICHEIRO ESCRITA. FLY_HASH.\n");
            exit(1);
        }

        *fly_first_time = true;
    }

    return fly_fp;
}


void add_airport(struct air_hash* air_hash, char code[AIRPORT], short GMT){
    struct airport* new = airport_new(code, GMT);

    if(air_search(air_hash, code) != NULL){
        printf("+ aeroporto %s existe\n", code);
        free(new);
        return;
    }

    air_insert(air_hash, new);
    printf("+ novo aeroporto %s\n", code);
}


void add_flight(struct air_hash* air_hash, struct fly_hash* fly_hash, struct flight* new){
    if(fly_search(fly_hash, new->name) != NULL){
        printf("+ voo %s existe\n", new->name);
        free(new);
        return;
    }

    if(air_search(air_hash, new->dep) == NULL){
        printf("+ aeroporto %s desconhecido\n", new->dep);
        free(new);
        return;
    }

    if(air_search(air_hash, new->dest) == NULL){
        printf("+ aeroporto %s desconhecido\n", new->dest);
        free(new);
        return;
    }

    int key = air_hashcode(new->dep);

    struct list_node* node = list_node_new(new);
    flight_list_insert(air_hash->flights[key], node);

    fly_insert(fly_hash, new);
    printf("+ novo voo %s\n", new->name);
}


void remove_flight(struct air_hash* air_hash, struct fly_hash* fly_hash, char flight_name[FLIGHT_CODE]){
    struct flight* fligth = fly_search(fly_hash, flight_name);

    if(fligth == NULL){
        printf("+ voo %s inexistente\n", flight_name);
        return;
    }

    int key = air_search_index(air_hash, fligth->dep);
    flight_list_remove(air_hash->flights[key], flight_name);
    free(fligth);
    printf("+ voo %s removido\n", flight_name);

}


void initialize_single_source(struct air_hash* air_hash, struct heap* heap, char start[AIRPORT]){

    struct vertice* new_vertice;
    struct airport* airport;
    int pos = 0;

    for(int i = 0; i < MAX_AIR_HASH; i++){
        airport = air_hash->table[i];

        if(airport != NULL){
            new_vertice = vertice_new(airport->code);
            min_heap_insert(air_hash, heap, new_vertice);
            airport->pos = pos;
            pos++;
        }
    }

    airport = air_search(air_hash, start);
    new_vertice = heap->array[airport->pos];
    new_vertice->distance = 0;
    heap_decrease_key(air_hash, heap, airport->pos,new_vertice);
}


void dijktra(struct air_hash* air_hash, struct fly_hash* fly_hash, struct heap* queue, short dep_time, char start[AIRPORT], char end[AIRPORT]){

    struct list_node* current_flight;                   //ADJ
    struct vertice* vertice1;                           //U
    struct vertice* vertice2;                           //V

    struct airport* airport1;
    struct airport* airport2;
    struct flight* flight1;

    int GMT_dif;
    int arrival_time;
    int time;                                           //W
    int key;

    initialize_single_source(air_hash, queue, start);

    airport1 = air_search(air_hash, start);
    short GMT_inicial = airport1->GMT;

    while(!heap_empty(queue)){

        vertice1 = heap_extract_min(air_hash, queue);     //remove da queue por ordem


        if(vertice1->visited) //vertice U ja´ foi visitado
            continue;


        if(strcmp(vertice1->name, start) == 0)
            arrival_time = dep_time;


            //calculo da hora de chegada ao ve'rtice U
        else{

            if(vertice1->parent == NULL)
                continue;

            flight1 = fly_search(fly_hash, vertice1->flight);
            airport2 = air_search(air_hash, vertice1->parent->name);
            GMT_dif = airport2->GMT - GMT_inicial;
            arrival_time = (dep_time + vertice1->distance + (GMT_dif)); //Pode estar errado
        }


        //acerto de hora
        if(arrival_time < 0)
            arrival_time = (short) (MIN_IN_H + arrival_time);

        if(arrival_time > MIN_IN_H)
            arrival_time = (short) (arrival_time % MIN_IN_H);




        //ADJ (U)
        key = air_search_index(air_hash, vertice1->name);
        current_flight = air_hash->flights[key]->header;

        while(current_flight != NULL){ //FOR ADJ U
            flight1 = current_flight->element;

            airport2 = air_search(air_hash, flight1->dest);
            vertice2 = queue->array[airport2->pos]; //V


            if(vertice2 == NULL){   //nunca deve acontecer
                current_flight = current_flight->next;
                continue;
            }

            //se U for o v'ertice inicial n~ao existe tempo de ligac~ao
            if(strcmp(vertice1->name, start) == 0)
                time = 0;

                //tempo de ligac~ao
            else
                time = CONNECTION_TIME;


            //o voo 'e depois da hora de chegada
            if(arrival_time + time <= flight1->dep_time){
                time = flight1->dep_time - arrival_time;    //tempo de espera
                time += flight1->duration;                  //durac~ao do voo
            }


                //espera um dia
            else{
                time = (short) (1440 - arrival_time + flight1->dep_time);
                time += flight1->duration;
            }



            //RELAX
            if(vertice1->distance + time < vertice2->distance){
                vertice2->distance = vertice1->distance + time;
                vertice2->parent = vertice1;
                strcpy(vertice2->flight, flight1->name);
                heap_decrease_key(air_hash, queue, airport2->pos, vertice2);
            }

                //Volta a introduzir na queue uma vez que pode n~ao ter sido alterado o valor da dist^ancia
            else
                min_heap_insert(air_hash, queue, vertice2);
            current_flight = current_flight->next;
        }

        vertice1->visited = true; //S + {U}


        if(strcmp(vertice1->name, end) == 0)
            break;
    }

}


void TR(struct air_hash* air_hash, struct fly_hash* fly_hash, short dep_time, char start[AIRPORT], char end[AIRPORT]){
    if(air_search(air_hash, start) == NULL){
        printf("+ aeroporto %s desconhecido\n", start);
        return;
    }

    if(air_search(air_hash, end) == NULL){
        printf("+ aeroporto %s desconhecido\n", end);
        return;
    }

    struct heap*
    dijktra(air_hash, fly_hash, dep_time, start, end);

}

int main(){

    FILE* air_fp = NULL;
    FILE* fly_fp = NULL;
    bool air_first_time = false;
    bool fly_first_time = false;

    air_fp = open_air_file(air_fp, &air_first_time);
    fly_fp = open_fly_file(fly_fp, &fly_first_time);


    char command[COMMAND_MAX];
    char airport_name1[AIRPORT];
    char airport_name2[AIRPORT];
    char flight_name[FLIGHT_CODE];
    short GMT_h, GMT_m, GMT, duration;
    bool flag = true;


    struct air_hash* air_hash = air_hash_new();
    struct fly_hash* fly_hash = fly_hash_new();

    struct flight* flight1;


    if(!air_first_time)
        air_read(air_fp, air_hash);

    if(!fly_first_time)
        fly_read(fly_fp, fly_hash, air_hash);


    while(flag){
        memset(command, '\0', COMMAND_MAX);
        memset(airport_name1, '\0', AIRPORT);
        memset(airport_name2, '\0', AIRPORT);
        memset(flight_name, '\0', FLIGHT_CODE);
        scanf("%s", command);

        if(strcmp(command, "AI") == 0){
            scanf("%s %hd:%hd", airport_name1, &GMT_h, &GMT_m);
            GMT = (short) (TIME(GMT_h, GMT_m));
            add_airport(air_hash, airport_name1, GMT);
        }


        else if(strcmp(command, "FI") == 0){
            scanf("%s %s %s %hd:%hd %hd", flight_name, airport_name1, airport_name2, &GMT_h, &GMT_m, &duration);
            GMT = (short) (TIME(GMT_h, GMT_m));

            flight1 = flight_new(flight_name, airport_name1, airport_name2, GMT, duration);
            add_flight(air_hash, fly_hash, flight1);
        }

        else if(strcmp(command, "FD") == 0){
            scanf("%s", flight_name);
            remove_flight(air_hash, fly_hash, flight_name);
        }

        else if(strcmp(command, "TR") == 0){
            scanf("%s %s %hd:%hd", airport_name1, airport_name2, &GMT_h, &GMT_m);
            GMT = (short) TIME(GMT_h, GMT_m);

        }

        else
            flag = false;
    }

    air_write(air_fp, air_hash, air_first_time);
    fly_write(fly_fp, fly_hash);
    return 0;
}