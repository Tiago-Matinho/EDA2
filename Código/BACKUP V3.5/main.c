#include "graph.h"

//TODO: escrita e leitura do disco
//TODO: GMT Continua mal. Só para o teste B

FILE* open_air_file(FILE* air_file){
    air_file = fopen(AIR_FILENAME, "r+");

    if(air_file == NULL){
        air_file = fopen(AIR_FILENAME, "w+");

        if(air_file == NULL){
            printf("ERRO A CRIAR FICHEIRO ESCRITA. AIR_HASH.\n");
            exit(1);
        }
        /*
        char blank_name[AIRPORT];
        memset(blank_name, '\0', AIRPORT);
        struct airport* blank = airport_new(blank_name, 0);
        for(int i = 0; i < MAX_AIRPORT; i++){
            fwrite(blank, sizeof(*blank), 1, air_file);
        }
         */
    }
    return air_file;
}

FILE* open_fly_file(FILE* fly_file){
    fly_file = fopen(FLY_FILENAME, "r+");

    if(fly_file == NULL){
        fly_file = fopen(FLY_FILENAME, "w+");

        if(fly_file == NULL){
            printf("ERRO A CRIAR FICHEIRO ESCRITA. FLY_HASH.\n");
            exit(1);
        }
        /*
        char blank_name[FLIGHT_CODE];
        char blank_name2[AIRPORT];
        memset(blank_name, '\0', FLIGHT_CODE);
        memset(blank_name2, '\0', AIRPORT);
        struct flight* blank = flight_new(blank_name, blank_name2, blank_name2, 0, 0);
        for(int i = 0; i < MAX_FLIGHT; i++){
            fwrite(blank, sizeof(*blank), 1, fly_file);
        }
         */
    }
    return fly_file;
}

FILE* open_flight_file(FILE* flight_list_file){
    flight_list_file = fopen(FLY_LIST_FILENAME, "r+");

    if(flight_list_file == NULL){
        flight_list_file = fopen(FLY_LIST_FILENAME, "w+");

        if(flight_list_file == NULL){
            printf("ERRO A CRIAR FICHEIRO ESCRITA. FLIGHT_LIST.\n");
            exit(1);
        }
        char blank_name[FLIGHT_CODE];
        memset(blank_name, '\0', FLIGHT_CODE);
        struct dep_node* blank = dep_node_new(blank_name);
        for(int i = 0; i < MAX_AIRPORT * 150; i++){
            fwrite(blank, sizeof(*blank), 1, flight_list_file);
        }
    }
    return flight_list_file;
}


void initialize_single_source(struct air_hash* air_hash, struct fly_hash* fly_hash, struct dist_list* queue,
                              struct vertice* start_vertice, struct visited_list* visited,
                              FILE* air_file, FILE* flight_list_file, FILE* fly_file){

    struct airport* airport1;
    struct vertice* new_vertice;
    struct flight* flight1;
    struct dep_node* current_flight = start_vertice->edges->header;

    while(current_flight != NULL){

        flight1 = fly_get(fly_hash, current_flight->name);

        if(flight1 == NULL){
            fly_read(fly_file, fly_hash, current_flight->name);
            flight1 = fly_get(fly_hash, current_flight->name);
        }

        //caso o ve'rtice n~ao exista
        if(!dist_list_exist(queue, flight1->dest) && !visited_list_exist(visited, flight1->dest)){
            //procura na RAM
            airport1 = air_get(air_hash, flight1->dest);

            //procura em Disco e insere em RAM caso exista
            if(airport1 == NULL){
                air_read(air_file, flight_list_file, fly_file, fly_hash, air_hash, flight1->dest);
                airport1 = air_get(air_hash, flight1->dest);
            }

            //cria o ve'rtice
            new_vertice = vertice_new(flight1->dest);
            new_vertice->edges = dep_list_new();
            new_vertice->edges->header = airport1->fly_list->header;
            dist_list_insert(queue, new_vertice); //insere na queue
        }

        current_flight = current_flight->next;
    }
}



struct path* dijkstra(struct air_hash* air_hash, struct fly_hash* fly_hash,
                      char start_name[AIRPORT], short dep_time, char end_name[AIRPORT],
                      FILE* air_file, FILE* flight_list_file, FILE* fly_file){

    struct dist_list* vertice_queue = dist_list_new();  //Q
    struct visited_list* visited = visited_list_new();  //S

    struct dep_node* current_flight;                    //ADJ
    struct vertice* vertice1;                           //U
    struct vertice* vertice2;                           //V

    struct airport* airport1;
    struct airport* airport2;
    struct flight* flight1;

    short GMT_dif;
    short arrival_time;
    short time;                                         //W


    //inserir o primeiro vertice
    airport1 = air_get(air_hash, start_name);
    vertice1 = vertice_new(start_name);
    vertice1->edges = dep_list_new();
    vertice1->edges->header = airport1->fly_list->header;
    vertice1->distance = 0;
    dist_list_insert(vertice_queue, vertice1);


    initialize_single_source(air_hash, fly_hash, vertice_queue, vertice1,
                             visited, air_file, flight_list_file, fly_file);

    short GMT_inicial = airport1->GMT;

    while(!dist_list_empty(vertice_queue)){

        vertice1 = dist_list_remove(vertice_queue);     //remove da queue por ordem

        if(visited_list_exist(visited, vertice1->name)) //vertice U ja´ foi visitado
            continue;

        airport1 = air_get(air_hash, vertice1->name);


        if(strcmp(vertice1->name, start_name) == 0)
            arrival_time = dep_time;


            //calculo da hora de chegada ao ve'rtice U
        else{
            flight1 = fly_get(fly_hash, vertice1->parent);
            airport2 = air_get(air_hash, flight1->dest);
            GMT_dif = airport2->GMT - GMT_inicial;
            arrival_time = (dep_time + vertice1->distance + (GMT_dif)); //Pode estar errado
        }



        //acerto de hora
        if(arrival_time < 0 || arrival_time > 1440)
            arrival_time = (short) (arrival_time % 1440);


        visited_list_insert(visited, vertice1); //S + {U}


        //inicialia-se os ve'rtices ligados a U
        initialize_single_source(air_hash, fly_hash, vertice_queue, vertice1, visited,
                                 air_file, flight_list_file, fly_file);


        //ADJ (U)
        current_flight = vertice1->edges->header;

        while(current_flight != NULL){ //FOR ADJ U
            flight1 = fly_get(fly_hash, current_flight->name);

            vertice2 = dist_list_get(vertice_queue, flight1->dest); //V


            if(vertice2 == NULL){   //caso V j'a tenha sido visitado
                current_flight = current_flight->next;
                continue;
            }

            //se U for o v'ertice inicial n~ao existe tempo de ligac~ao
            if(strcmp(vertice1->name, start_name) == 0)
                time = 0;

                //tempo de ligac~ao
            else
                time = 30;


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
                strcpy(vertice2->parent, current_flight->name);
            }

            //Volta a introduzir na queue uma vez que pode ter sido alterado o valor da dist^ancia
            dist_list_insert(vertice_queue, vertice2);
            current_flight = current_flight->next;
        }
    }


    //constroi o caminho de volta
    struct path* final = path_new();

    //v'ertice de chegada
    vertice1 = visited_list_get(visited, end_name);

    //n~ao existe caminho
    if(vertice1 == NULL)
        return NULL;


    path_insert(final, vertice1);


    //enquanto n~ao existir caminho de volta
    while(vertice1->parent[0] != '\0'){

        flight1 = fly_get(fly_hash, vertice1->parent);

        vertice1 = visited_list_get(visited, flight1->dep);

        if(vertice1 == NULL)
            printf("Erro a encontrar vértice %s para construir caminho final\n", flight1->dep);

        path_insert(final, vertice1);
    }

    dist_list_destroy(vertice_queue);
    visited_list_destroy(visited);
    return final;
}


int main(){
    FILE* air_file = NULL;
    FILE* fly_file = NULL;
    FILE* flight_list_file = NULL;
    air_file = open_air_file(air_file);
    fly_file = open_fly_file(fly_file);
    flight_list_file = open_flight_file(flight_list_file);

    bool flag = true;

    struct air_hash* air_hash = air_hash_new();
    struct fly_hash* fly_hash = fly_hash_new();

    struct dep_list* dep_list1;
    struct dep_node* dep_node1;

    struct flight* flight_node;
    struct airport* airport_node1;
    struct airport* airport_node2;

    struct path* path;
    struct vertice* vertice1;

    char command[3] = {'\0'};
    char airport1[AIRPORT] = {'\0'};
    char airport2[AIRPORT] = {'\0'};
    char flight_code[FLIGHT_CODE] = {'\0'};
    short GMT_h, GMT_m, GMT, duration;
    short GMT_h2, GMT_m2;
    short GMT_dif;


    while(flag){
        scanf("%s", command);

        if(strcmp(command, "AI") == 0){
            scanf("%s %hd:%hd", airport1, &GMT_h, &GMT_m);

            if(GMT_h < 0)
                GMT_m = (short) (GMT_m * -1);

            GMT = (short) (TIME(GMT_h, GMT_m));

            //procura em RAM
            airport_node1 = air_get(air_hash, airport1);

            //procura em Disco e insere em RAM caso exista
            if(airport_node1 == NULL){
                air_read(air_file, flight_list_file, fly_file, fly_hash, air_hash, airport1);
                airport_node1 = air_get(air_hash, airport1);
            }

            if(airport_node1 != NULL)
                printf("+ aeroporto %s existe\n", airport1);


            else{
                airport_node1 = airport_new(airport1, GMT);
                air_insert(air_hash, airport_node1);
                printf("+ novo aeroporto %s\n", airport1);
            }
        }

        else if(strcmp(command, "FI") == 0){
            scanf("%s %s %s %hd:%hd %hd", flight_code, airport1, airport2, &GMT_h, &GMT_m, &duration);

            if(GMT_h < 0)
                GMT_m = (short) (GMT_m * -1);

            GMT = (short) (TIME(GMT_h, GMT_m));

            //procura em RAM
            flight_node = fly_get(fly_hash, flight_code);

            //procura em Disco e insere em RAM caso exista
            if(flight_node == NULL){
                fly_read(fly_file, fly_hash, flight_code);
                flight_node = fly_get(fly_hash, flight_code);
            }

            if(flight_node != NULL){
                printf("+ voo %s existe\n", flight_code);
                continue;
            }

            //procura o primeiro aeroporto em RAM
            airport_node1 = air_get(air_hash, airport1);

            //procura em Disco e insere em RAM caso exista
            if(airport_node1 == NULL){
                air_read(air_file, flight_list_file, fly_file, fly_hash, air_hash, airport1);
                airport_node1 = air_get(air_hash, airport1);
            }

            if(airport_node1 == NULL){
                printf("+ aeroporto %s desconhecido\n", airport1);
                continue;
            }

            //procura o primeiro aeroporto em RAM
            airport_node2 = air_get(air_hash, airport2);

            //procura em Disco e insere em RAM caso exista
            if(airport_node2 == NULL){
                air_read(air_file, flight_list_file, fly_file, fly_hash, air_hash, airport2);
                airport_node2 = air_get(air_hash, airport2);
            }

            if(airport_node2 == NULL)
                printf("+ aeroporto %s desconhecido\n", airport2);


            else{
                flight_node = flight_new(flight_code, airport1, airport2, GMT, duration);
                fly_insert(fly_hash, flight_node);

                dep_list1 = airport_node1->fly_list;
                dep_node1 = dep_node_new(flight_code);
                dep_list_insert(dep_list1, dep_node1);
                printf("+ novo voo %s\n", flight_code);
            }
        }

        else if(strcmp(command, "FD") == 0){
            scanf("%s", flight_code);

            //procura em RAM
            flight_node = fly_get(fly_hash, flight_code);

            //procura em Disco e insere em RAM caso exista
            if(flight_node == NULL){
                fly_read(fly_file, fly_hash, flight_code);
                flight_node = fly_get(fly_hash, flight_code);
            }

            if(flight_node == NULL){
                printf("+ voo %s inexistente\n", flight_code);
                continue;
            }

            //procura em RAM
            airport_node1 = air_get(air_hash, flight_node->dep);

            //procura em Disco e insere em RAM caso exista
            if(airport_node1 == NULL){
                air_read(air_file, flight_list_file, fly_file, fly_hash, air_hash, airport1);
                airport_node1 = air_get(air_hash, airport1);
            }

            //remove da lista de voos
            dep_list_remove(airport_node1->fly_list, flight_code);
            flight_node->erased = true; //marca para remoc~ao no disco
            printf("+ voo %s removido\n", flight_code);
        }

        else if(strcmp(command, "TR") == 0){
            scanf("%s %s %hd:%hd", airport1, airport2, &GMT_h, &GMT_m);
            GMT = (short) (TIME(GMT_h, GMT_m));

            //procura em RAM
            airport_node1 = air_get(air_hash, airport1);

            //procura em Disco e insere em RAM caso exista
            if(airport_node1 == NULL){
                air_read(air_file, flight_list_file, fly_file, fly_hash, air_hash, airport1);
                airport_node1 = air_get(air_hash, airport1);
            }

            if(airport_node1 == NULL){
                printf("+ aeroporto %s desconhecido\n", airport1);
                continue;
            }

            //procura em RAM
            airport_node2 = air_get(air_hash, airport2);

            //procura em Disco e insere em RAM caso exista
            if(airport_node2 == NULL){
                air_read(air_file, flight_list_file, fly_file, fly_hash, air_hash, airport2);
                airport_node2 = air_get(air_hash, airport2);
            }

            if(airport_node2 == NULL){
                printf("+ aeroporto %s desconhecido\n", airport2);
                continue;
            }


            path = dijkstra(air_hash, fly_hash, airport1, GMT, airport2,
                            air_file, flight_list_file, fly_file);

            if(path == NULL){
                printf("+ sem voos de %s para %s\n", airport1, airport2);
                continue;
            }
            vertice1 = path->header;

            //errado???
            if(strcmp(vertice1->name, airport1) != 0) {
                printf("+ sem voos de %s para %s\n", airport1, airport2);
                continue;
            }


            duration = 0;
            printf("Voo    De   Para Parte Chega\n");
            printf("====== ==== ==== ===== =====\n");
            while(vertice1->next != NULL){
                vertice1 = vertice1->next;
                flight_node = fly_get(fly_hash, vertice1->parent);

                airport_node1 = air_get(air_hash, flight_node->dep);
                airport_node2 = air_get(air_hash, flight_node->dest);

                GMT_dif = airport_node2->GMT - airport_node1->GMT;

                GMT = flight_node->dep_time;
                if(GMT > 1440)
                    GMT = (short) (GMT % 1440);


                GMT_h = (short)(GMT / 60);
                GMT_m = (short)(GMT % 60);

                if(GMT_h < 0)
                    GMT_m = (short) ((GMT * -1) % 60);

                if(GMT_dif > 0 && GMT_dif < 1440)
                    GMT_dif %= 1440;

                GMT += flight_node->duration + (GMT_dif);

                if(GMT > 1440)
                    GMT = (short) (GMT % 1440);

                if(GMT < 0)
                    GMT = (short) ((1440 + GMT) % 1440);

                GMT_h2 = (short)(GMT / 60);
                GMT_m2 = (short)(GMT % 60);

                if(GMT_h < 0)
                    GMT_m = (short) ((GMT * -1) % 60);

                printf("%-6s %-4s %-4s %02hd:%02hd %02hd:%02hd\n", flight_node->name, flight_node->dep, flight_node->dest, GMT_h, GMT_m, GMT_h2, GMT_m2);
            }
            duration = vertice1->distance;
            printf("Tempo de viagem: %hd minutos\n", duration);
            path_list_destroy(path);

        }

        else
            flag = false;

        memset(airport1, '\0', AIRPORT);
        memset(airport2, '\0', AIRPORT);
    }

    air_write(air_file, flight_list_file, air_hash);
    fly_write(fly_file, fly_hash);

    return 0;
}