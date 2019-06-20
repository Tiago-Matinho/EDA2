#include "graph.h"


void initialize_single_source(struct air_hash* air_hash, struct dist_list* queue,
                              struct vertice* start_vertice, short dep_time){

    struct airport* airport1;
    struct vertice* new_vertice;
    struct dep_node* current_flight = start_vertice->edges->header;

    while(current_flight != NULL){
        new_vertice = dist_list_get(queue, current_flight->dest);

        if(new_vertice == NULL){ //se o vertice ainda não existe
            airport1 = air_get(air_hash, current_flight->dest);
            new_vertice = vertice_new(current_flight->dest);
            new_vertice->edges = dep_list_new();
            new_vertice->edges->header = airport1->fly_list->header;
            dist_list_insert(queue, new_vertice);
        }

        current_flight = current_flight->next;
    }
}



struct path* dijkstra(struct air_hash* air_hash, struct fly_hash* fly_hash, char start_name[AIRPORT], short dep_time, char end_name[AIRPORT]){

    struct dist_list* vertice_queue = dist_list_new();  //Q
    struct visited_list* visited = visited_list_new();  //S
    //struct dep_list* edge_list;

    struct dep_node* current_flight;                    //ADJ
    struct vertice* vertice1;                           //U
    struct vertice* vertice2;                           //V

    struct airport* airport1;

    short arrival_time;
    short time;                                         //W

    //inserir o primeiro vertice
    airport1 = air_get(air_hash, start_name);
    vertice1 = vertice_new(airport1->name);
    vertice1->edges = dep_list_new();
    vertice1->edges->header = airport1->fly_list->header;
    vertice1->distance = 0;
    dist_list_insert(vertice_queue, vertice1);
    arrival_time = dep_time;

    while(!dist_list_empty(vertice_queue)){
        vertice1 = dist_list_remove(vertice_queue);
        airport1 = air_get(air_hash, vertice1->name);

        if(strcmp(vertice1->name, start_name) != 0)
            arrival_time =(short) (dep_time + vertice1->distance + 30 + airport1->GMT);

        if(visited_list_exist(visited, vertice1->name)) //vertice U ja´ foi visitado
            continue;


        initialize_single_source(air_hash, vertice_queue, vertice1, arrival_time);
        current_flight = vertice1->edges->header;


        while(current_flight != NULL){
            vertice2 = dist_list_get(vertice_queue, current_flight->dest);

            if(vertice2 == NULL)
                printf("merda\n");

            if(arrival_time <= current_flight->dep_time){
                time = current_flight->dep_time - arrival_time;
                time += current_flight->duration;
            }

            else{
                time = (short) (1440 - arrival_time + current_flight->dep_time);
                time += current_flight->duration + 30;
            }

            if(vertice1->distance + time < vertice2->distance){
                vertice2->distance = vertice1->distance + time;
                strcpy(vertice2->parent, current_flight->name);
            }

            dist_list_insert(vertice_queue, vertice2);
            current_flight = current_flight->next;
        }

        visited_list_insert(visited, vertice1);
    }


    struct path* final = path_new();
    struct flight* flight1;

    vertice1 = visited_list_get(visited, end_name);
    if(vertice1 == NULL)
        return NULL;

    path_insert(final, vertice1);

    while(vertice1->parent[0] != '\0'){
        flight1 = fly_get(fly_hash, vertice1->parent);
        vertice1 = visited_list_get(visited, flight1->air_dep);
        path_insert(final, vertice1);
    }
    return final;
}


int main(){

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


    while(flag){
        scanf("%s", command);

        if(strcmp(command, "AI") == 0){
            scanf("%s %hd:%hd", airport1, &GMT_h, &GMT_m);
            GMT = (short) (TIME(GMT_h, GMT_m));

            airport_node1 = air_get(air_hash, airport1);

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
            GMT = (short) (TIME(GMT_h, GMT_m));

            flight_node = fly_get(fly_hash, flight_code);

            if(flight_node != NULL){
                printf("+ voo %s existe\n", flight_code);
                continue;
            }

            airport_node1 = air_get(air_hash, airport1);

            if(airport_node1 == NULL){
                printf("+ aeroporto %s desconhecido\n", airport1);
                continue;
            }

            airport_node2 = air_get(air_hash, airport2);

            if(airport_node2 == NULL)
                printf("+ aeroporto %s desconhecido\n", airport2);

            else{
                flight_node = flight_new(flight_code, airport1, airport2, GMT, duration);
                fly_insert(fly_hash, flight_node);

                dep_list1 = airport_node1->fly_list;
                dep_node1 = dep_node_new(flight_code, airport2, GMT, duration);
                dep_list_insert2(dep_list1, dep_node1);
                printf("+ novo voo %s\n", flight_code);
            }
        }

        else if(strcmp(command, "FD") == 0){
            scanf("%s", flight_code);

            flight_node = fly_get(fly_hash, flight_code);

            if(flight_node == NULL){
                printf("+ voo %s inexistente\n", flight_code);
                continue;
            }

            airport_node1 = air_get(air_hash, flight_node->air_dep);

            dep_list_remove2(airport_node1->fly_list, flight_code);
            flight_node->erased = true;
            printf("+ voo %s removido\n", flight_code);
        }

        else if(strcmp(command, "TR") == 0){
            scanf("%s %s %hd:%hd", airport1, airport2, &GMT_h, &GMT_m);
            GMT = (short) (TIME(GMT_h, GMT_m));

            airport_node1 = air_get(air_hash, airport1);
            airport_node2 = air_get(air_hash, airport2);

            if(airport_node1 == NULL){
                printf("+ aeroporto %s desconhecido\n", airport1);
                continue;
            }

            if(airport_node2 == NULL){
                printf("+ aeroporto %s desconhecido\n", airport2);
                continue;
            }

            if(strcmp(airport1, airport2) == 0){
                printf("Voo    De   Para Parte Chega\n");
                printf("====== ==== ==== ===== =====\n");
                continue;
            }

            path = dijkstra(air_hash, fly_hash, airport1, GMT, airport2);

            if(path == NULL){
                printf("+ sem voos de %s para %s\n", airport1, airport2);
                continue;
            }
            vertice1 = path->header;
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
                GMT = flight_node->dep_time;
                GMT_h = (short)(GMT / 60);
                GMT_m = (short)(GMT % 60);
                GMT += flight_node->duration;
                GMT_h2 = (short)(GMT / 60);
                GMT_m2 = (short)(GMT % 60);

                printf("%s %s %s %02hd:%02hd %02hd:%02hd\n", flight_node->name, flight_node->air_dep, flight_node->air_dest, GMT_h, GMT_m, GMT_h2, GMT_m2);
            }
            duration += vertice1->distance;
            printf("Tempo de viagem: %hd minutos\n", duration);

        }

        else
            flag = false;
    }


    return 0;
}