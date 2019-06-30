#include "path.h"


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

    long int key = air_search_index(air_hash, new->dep);

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

    long int key = air_search_index(air_hash, fligth->dep);
    flight_list_remove(air_hash->flights[key], flight_name);
    long int key2 = fly_search_index(fly_hash, flight_name);
    free(fligth);
    fly_hash->table[key2] = NULL;
    printf("+ voo %s removido\n", flight_name);

}


void initialize_single_source(struct air_hash* air_hash, struct heap* heap, char start[AIRPORT]){

    struct vertice* new_vertice;
    struct airport* airport;

    for(int i = 0; i < MAX_AIR_HASH; i++){
        new_vertice = air_hash->vertices[i];

        if(new_vertice != NULL){
            new_vertice->parent = NULL;
            new_vertice->distance = INF;

            min_heap_insert(air_hash, heap, new_vertice);
        }
    }

    //printf("teste\n");
    airport = air_search(air_hash, start);
    new_vertice = heap->array[airport->pos];
    new_vertice->distance = 0;
   change_value(air_hash, heap, airport->pos);
   }


struct vertice* dijktra(struct air_hash* air_hash, struct heap* queue,
                        short dep_time, char start[AIRPORT], char end[AIRPORT]){

    struct list_node* current_flight;                   //ADJ
    struct vertice* vertice1;                           //U
    struct vertice* vertice2;                           //V

    struct airport* airport1;
    struct airport* airport2;
    struct flight* flight1;

    int GMT_dif;
    int arrival_time;
    int time;                                           //W
    long int key;

    initialize_single_source(air_hash, queue, start);

    airport1 = air_search(air_hash, start);
    short GMT_inicial = airport1->GMT;

    while(!heap_empty(queue)){

        vertice1 = heap_extract_min(air_hash, queue);     //remove da queue por ordem


        if(strcmp(vertice1->name, start) == 0)
            arrival_time = dep_time;


            //calculo da hora de chegada ao ve'rtice U
        else{

            if(vertice1->parent == NULL)
                continue;

            airport1 = air_search(air_hash, vertice1->name);
            GMT_dif = airport1->GMT - GMT_inicial;
            arrival_time = (dep_time + vertice1->distance + (GMT_dif));
        }


        //acerto de hora
        if(arrival_time > MIN_IN_DAY)
            arrival_time = (short) (arrival_time % MIN_IN_DAY);

        if(arrival_time < 0)
            arrival_time = (short) (MIN_IN_DAY + arrival_time);


        //ADJ (U)
        key = air_search_index(air_hash, vertice1->name);
        current_flight = air_hash->flights[key]->header;

        while(current_flight != NULL){ //FOR ADJ U
            flight1 = current_flight->element;

            airport2 = air_search(air_hash, flight1->dest);


            if(airport2->pos == -1){ //foi visitado
                current_flight = current_flight->next;
                continue;
            }

            vertice2 = queue->array[airport2->pos]; //V



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
                time = (short) (MIN_IN_DAY - arrival_time + flight1->dep_time);
                time += flight1->duration;
            }


            //RELAX
            if(vertice1->distance + time < vertice2->distance){
                vertice2->distance = vertice1->distance + time;
                vertice2->parent = flight1;
                change_value(air_hash, queue, airport2->pos);
            }

            current_flight = current_flight->next;
        }


        if(strcmp(vertice1->name, end) == 0)
            return vertice1;
    }

    return NULL;
}


void build_path(struct air_hash* air_hash, struct heap* heap, struct vertice* end_vertice){

    struct flight_list* path = flight_list_new();
    struct list_node* node;
    struct vertice* current = end_vertice;
    struct airport* airport1;
    struct airport* airport2;


    while(current->parent != NULL){
        node = list_node_new(current->parent);
        path_insert(path, node);

        current = air_hash->vertices[air_search_index(air_hash, current->parent->dep)];
    }


    short GMT_dif;
    short time, time1_h, time1_m, time2_h, time2_m;


    while(!flight_list_empty(path)){
        node = path_remove(path);

        airport1 = air_search(air_hash, node->element->dep);
        airport2 = air_search(air_hash, node->element->dest);

        GMT_dif = airport2->GMT - airport1->GMT;

        time = node->element->dep_time;

        if(time > MIN_IN_DAY)
            time = (short) (time % MIN_IN_DAY);


        time1_h = (short) TO_GMT_H(time);
        time1_m = (short) TO_GMT_M(time);

        if(time1_h < 0)
            time1_m = (short) ((time * -1) % 60);

        if(GMT_dif > 0 && GMT_dif < MIN_IN_DAY)
            GMT_dif %= MIN_IN_DAY;

        time += node->element->duration + (GMT_dif);

        if(time > MIN_IN_DAY)
            time = (short) (time % MIN_IN_DAY);

        if(time < 0)
            time = (short) ((MIN_IN_DAY + time) % MIN_IN_DAY);

        time2_h = (short)TO_GMT_H(time);
        time2_m = (short)TO_GMT_M(time);



        printf("%-6s %-4s %-4s %02hd:%02hd %02hd:%02hd\n", node->element->name, node->element->dep, node->element->dest, time1_h, time1_m, time2_h, time2_m);
        free(node);
    }
    flight_list_destroy(path);
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

    struct heap* queue = heap_new();
    struct vertice* end_vertice;


    end_vertice = dijktra(air_hash, queue, dep_time, start, end);


    if(end_vertice == NULL){
        printf("+ sem voos de %s para %s\n", start, end);
        free(queue);
        return;
    }

    if(strcmp(end_vertice->name, end) != 0){
        printf("+ sem voos de %s para %s\n", start, end);
        min_heap_destroy(queue);
        return;
    printf("Voo    De   Para Parte Chega\n");
    printf("====== ==== ==== ===== =====\n");
    build_path(air_hash, queue, end_vertice);
    printf("Tempo de viagem: %d minutos\n", end_vertice->distance);

    min_heap_destroy(queue);
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

            if(GMT_h < 0)
                GMT_m = (short) (GMT_m * -1);

            GMT = (short) (TIME(GMT_h, GMT_m));
            add_airport(air_hash, airport_name1, GMT);
        }


        else if(strcmp(command, "FI") == 0){
            scanf("%s %s %s %hd:%hd %hd", flight_name, airport_name1, airport_name2, &GMT_h, &GMT_m, &duration);

            if(GMT_h < 0)
                GMT_m = (short) (GMT_m * -1);

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

            TR(air_hash, fly_hash, GMT, airport_name1, airport_name2);
        }

        else
            flag = false;
    }

    air_write(air_fp, air_hash);
    fly_write(fly_fp, fly_hash);
    return 0;
}