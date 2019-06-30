#include "min_heap.h"

/*****************************************************************************+
+ Abre o ficheiro que guarda a informac~ao da Hashtable relativa aos          +
+ aeroportos.                                                                 +
+                                                                             +
+ Caso n~ao exista cria e muda o valor do air_first_time representando        +
+ que foi criado.                                                             +
+*****************************************************************************/
FILE* open_air_file(FILE* air_fp, bool* air_first_time){
    air_fp = fopen(AIR_FILENAME, "r+");

    if(air_fp == NULL){ //caso n~ao exista
        air_fp = fopen(AIR_FILENAME, "w+");

        if(air_fp == NULL){
            printf("ERRO A CRIAR FICHEIRO ESCRITA. AIR_HASH.\n");
            exit(1);
        }

        *air_first_time = true;
    }
    return air_fp;
}


/*****************************************************************************+
+ Abre o ficheiro que guarda a informac~ao da Hashtable relativa aos          +
+ voos.                                                                       +
+                                                                             +
+ Caso n~ao exista cria e muda o valor do fly_first_time representando        +
+ que foi criado.                                                             +
+*****************************************************************************/
FILE* open_fly_file(FILE* fly_fp, bool* fly_first_time){
    fly_fp = fopen(FLY_FILENAME, "r+");

    if(fly_fp == NULL){ //caso n~ao exista
        fly_fp = fopen(FLY_FILENAME, "w+");

        if(fly_fp == NULL){
            printf("ERRO A CRIAR FICHEIRO ESCRITA. FLY_HASH.\n");
            exit(1);
        }

        *fly_first_time = true;
    }

    return fly_fp;
}


/*****************************************************************************+
+ Procura se o aeroporto a ser adicionado ao sistema j'a existe. Se existir   +
+ imprime no standard output que o aeroporto j'a existe.                      +
+                                                                             +
+ Caso n~ao exista adiciona o aeroporto `a hashtable e imprime no standard      +
+ output uma mensagem a dizer que foi criado.                                 +
+*****************************************************************************/
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


/*****************************************************************************+
+ Procura se o voo a ser adicionado ao sistema j'a existe. Se existir         +
+ imprime no standard output que o voo j'a existe.                            +
+                                                                             +
+ Se não existir procura se ambos o aeroporto de chegada e o de partida       +
+ existem. Se n~ao existirem imprime uma mensagem a dizer q os aeroportos     +
+ s~ao desconhecidos.                                                         +
+                                                                             +
+ Caso n~ao exista adiciona o voo `a hashtable de voos,  adiciona `a lista de +
+ voos de partida do aeroporto de partida e imprime no standard               +
+ output uma mensagem a dizer que foi criado.                                 +
+*****************************************************************************/
void add_flight(struct air_hash* air_hash, struct fly_hash* fly_hash,
    struct flight* new){

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

    //adiciona `a lista de voos que partem do aeroporto
    struct list_node* node = list_node_new(new);
    flight_list_insert(air_hash->flights[key], node);

    //adiciona `a hashtable de voos
    fly_insert(fly_hash, new);
    printf("+ novo voo %s\n", new->name);
}

/*****************************************************************************+
+ Procura se o voo a ser removido existe na hashtable de voos.                +
+                                                                             +
+ Se não existir imprime no standard output que o voo n~ao existe.            +
+                                                                             +
+ Caso exista remove o voo da lista de voos que partem do aeroporto de        +
+ partida e remove-o tamb'em da hashtable de voos.                            +
+*****************************************************************************/
void remove_flight(struct air_hash* air_hash, struct fly_hash* fly_hash,
    char flight_name[FLIGHT_CODE]){

    struct flight* fligth = fly_search(fly_hash, flight_name);

    if(fligth == NULL){
        printf("+ voo %s inexistente\n", flight_name);
        return;
    }

    long int key = air_search_index(air_hash, fligth->dep);
    long int key2 = fly_search_index(fly_hash, flight_name);

    //remove o voo da lista de voos que partem do aeroporto de partida
    flight_list_remove(air_hash->flights[key], flight_name);
    free(fligth); //liberta o voo da hashtable de voos
    fly_hash->table[key2] = NULL;
    printf("+ voo %s removido\n", flight_name);

}


/*****************************************************************************+
+ Inicializa todos os v'ertices com parent = NIL e distancia = infinito.      +
+                                                                             +
+ Insere os v'ertices numa lista de prioridades (uma min-heap).               +
+*****************************************************************************/
void initialize_single_source(struct air_hash* air_hash, struct heap* heap,
    char start[AIRPORT]){

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

    airport = air_search(air_hash, start);
    new_vertice = heap->array[airport->pos];
    new_vertice->distance = 0;
   change_value(air_hash, heap, airport->pos);
}


/*****************************************************************************+
+ implementac~ao do algoritmo de Dijkstra baseada no livro                    +
+ escrito por: Cormen T, Leiserson C, Rivest R, Stein C   (2009)              +
+ "Introduction to Algorithms" página 151.                                    +
******************************************************************************/
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

        //remove da queue por ordem
        vertice1 = heap_extract_min(air_hash, queue);


        //caso seja o aeroporto de partida a hora de chegada 'e a dep_time
        if(strcmp(vertice1->name, start) == 0)
            arrival_time = dep_time;


            //c'alculo da hora de chegada ao ve'rtice U
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


/*****************************************************************************+
+ Constr'oi uma lista ligada com os voos que fazem o percurso resultante do   +
+ algoritmo de Dijkstra e imprime-o.                                          +
+*****************************************************************************/
void build_path(struct air_hash* air_hash, struct vertice* end_vertice){

    struct flight_list* path = flight_list_new();
    struct list_node* node;
    struct vertice* current = end_vertice;
    struct airport* airport1;
    struct airport* airport2;


    //constr'oi o caminho desde o 'ultimo v'ertice at'e ao inicial por ordem
    //de in'icio para o fim
    while(current->parent != NULL){
        node = list_node_new(current->parent);
        flight_list_insert(path, node);

        current = air_hash->vertices[air_search_index(air_hash,
            current->parent->dep)];
    }


    short GMT_dif;
    short time, time1_h, time1_m, time2_h, time2_m;


    while(!flight_list_empty(path)){
        node = flight_list_pop(path);

        airport1 = air_search(air_hash, node->element->dep);
        airport2 = air_search(air_hash, node->element->dest);

        GMT_dif = airport2->GMT - airport1->GMT;

        time = node->element->dep_time; //hora de partida

        //acerto de hora
        if(time > MIN_IN_DAY)
            time = (short) (time % MIN_IN_DAY);


        time1_h = (short) TO_GMT_H(time);
        time1_m = (short) TO_GMT_M(time);

        if(time1_h < 0)
            time1_m = (short) ((time * -1) % 60);

        if(GMT_dif > 0 && GMT_dif < MIN_IN_DAY)
            GMT_dif %= MIN_IN_DAY;

        //hora de chegada
        time += node->element->duration + (GMT_dif);

        //acerto de hora
        if(time > MIN_IN_DAY)
            time = (short) (time % MIN_IN_DAY);

        if(time < 0)
            time = (short) ((MIN_IN_DAY + time) % MIN_IN_DAY);

        time2_h = (short)TO_GMT_H(time);
        time2_m = (short)TO_GMT_M(time);


        printf("%-6s %-4s %-4s %02hd:%02hd %02hd:%02hd\n", node->element->name,
            node->element->dep, node->element->dest, time1_h, time1_m, time2_h, time2_m);
        free(node);
    }

    //liberta a lista da mem'oria
    flight_list_destroy(path);
}


/*****************************************************************************+
+ C'alcula a viagem com menos tempo de durac~ao entre dois aeroportos.
+ 
+ Caso n~ao encontre os aeroportos imprime no standard output
+ que s~ao desconhecidos.
+
+ Se não aplica Dijkstra e imprime o caminho.
+*****************************************************************************/
void TR(struct air_hash* air_hash, struct heap* heap, short dep_time,
    char start[AIRPORT], char end[AIRPORT]){

    if(air_search(air_hash, start) == NULL){
        printf("+ aeroporto %s desconhecido\n", start);
        return;
    }

    if(air_search(air_hash, end) == NULL){
        printf("+ aeroporto %s desconhecido\n", end);
        return;
    }


    struct vertice* end_vertice;


    //recebe o v'ertice final depois de aplicado dijkstra
    end_vertice = dijktra(air_hash, heap, dep_time, start, end);


    if(end_vertice == NULL){
        printf("+ sem voos de %s para %s\n", start, end);
        heap->size = 0;
        return;
    }

    //caso o vertice final n~ao seja o suposto
    if(strcmp(end_vertice->name, end) != 0) {
        printf("+ sem voos de %s para %s\n", start, end);
        heap->size = 0;
        return;
    }

    //imprime o caminho
    printf("Voo    De   Para Parte Chega\n");
    printf("====== ==== ==== ===== =====\n");
    build_path(air_hash, end_vertice);
    printf("Tempo de viagem: %d minutos\n", end_vertice->distance);

    heap->size = 0;
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

    struct heap* heap = heap_new();

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

            //acerto do GMT para GMT's negativos
            if(GMT_h < 0)
                GMT_m = (short) (GMT_m * -1);

            GMT = (short) (TIME(GMT_h, GMT_m));
            add_airport(air_hash, airport_name1, GMT);
        }


        else if(strcmp(command, "FI") == 0){
            scanf("%s %s %s %hd:%hd %hd", flight_name, airport_name1, airport_name2, &GMT_h, &GMT_m, &duration);

            //acerto do GMT para GMT's negativos
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

            TR(air_hash, heap, GMT, airport_name1, airport_name2);
        }

        else
            flag = false;
    }

    air_write(air_fp, air_hash);
    fly_write(fly_fp, fly_hash);
    free(heap);
    return 0;
}