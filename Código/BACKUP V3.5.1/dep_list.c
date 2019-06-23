#include "dep_list.h"


struct dep_node* dep_node_new(char name[FLIGHT_CODE]){
    struct dep_node* new_node = malloc(sizeof(struct dep_node));

    if(new_node != NULL){
        memset(new_node->name, '\0', FLIGHT_CODE);
        strcpy(new_node->name, name);
        new_node->next = NULL;
    }
    return new_node;
}


struct dep_list* dep_list_new(){
    struct dep_list* new_list = malloc(sizeof(struct dep_list));

    if(new_list != NULL){
        new_list->header = NULL;
        new_list->changed = false;
    }

    return new_list;
}


/*-------------------------------------------------------------------*/


bool dep_list_empty(struct dep_list* list){
    return list->header == NULL;
}


void dep_list_insert(struct dep_list* list, struct dep_node* new_node){

    list->changed = true;

    //first insertion
    if(dep_list_empty(list)){
        list->header = new_node;
        return;
    }

    struct dep_node* current = list->header;

    //insert on head
    if(strcmp(current->name, new_node->name) >= 0){
        new_node->next = current;
        list->header = new_node;
        return;
    }

    while(current->next != NULL){

        if(strcmp(current->next->name, new_node->name) >= 0)
            break;

        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}


void dep_list_remove(struct dep_list* list, char name[FLIGHT_CODE]){
    list->changed = true;

    //nothing to remove
    if(dep_list_empty(list))
        return;

    struct dep_node* current = list->header;
    struct dep_node* aux;

    //one element list
    if(current->next == NULL && strcmp(current->name, name) == 0){
        free(current);
        list->header = NULL;
        return;
    }

    if(strcmp(name, current->name) == 0){
        list->header = current->next;
        free(current);
        return;
    }

    while(current->next != NULL){

        if(strcmp(current->next->name, name) == 0){
            aux = current->next;
            current->next = aux->next;
            free(aux);
            return;
        }

        if(strcmp(current->next->name, name) > 0)
            return;

        current = current->next;
    }
}


struct dep_node* dep_list_search(struct dep_list* list, char name[FLIGHT_CODE]){
    if(dep_list_empty(list))
        return NULL;

    struct dep_node* current = list->header;

    while(current->next != NULL){

        if(strcmp(current->next->name, name) == 0)
            return current->next;

        if(strcmp(current->next->name, name) > 0)
            return NULL;

        current = current->next;
    }
    return NULL;
}

/*-------------------------------------------------------------------*/

void dep_list_read(FILE* flight_list_file, FILE* fly_file, struct fly_hash* fly_hash, struct dep_list* list, int key){
    if(flight_list_file == NULL){
        printf("ERRO A LER DEP_LIST\n");
        exit(1);
    }

    if(list == NULL){
        printf("ERRO LISTA VAZIA. DEP_LIST\n");
        exit(1);
    }

    int ind = key * 150;
    struct dep_node* current;
    struct dep_node* aux;

    fseek(flight_list_file, ind * sizeof(*current), SEEK_SET);

    current = malloc(sizeof(struct dep_node));
    fread(current, sizeof(*current), 1, flight_list_file);

    if(current->name[0] == '\0'){
        free(current);
        return;
    }

    aux = current;
    list->header = aux;

    for(int i = 1; i < 150; i++){
        current = malloc(sizeof(struct dep_node));
        fread(current, sizeof(*current), 1, flight_list_file);

        if(current->name[0] == '\0'){
            free(current);
            return;
        }

        fly_read(fly_file, fly_hash, current->name);
        aux->next = current;
        aux = current;
    }
}


void dep_list_write(FILE* flight_list_file, struct dep_list* list, int key){
    if(flight_list_file == NULL){
        printf("ERRO A ESCREVER DEP_LIST\n");
        exit(1);
    }

    if(!list->changed)
        return;

    int ind = key * 150;
    struct dep_node* current = list->header;
    struct dep_node* aux;

    fseek(flight_list_file, ind * sizeof(*current), SEEK_SET);

    int i;
    for(i = 0; i < 150; i++){

        if(current == NULL)
            break;

        aux = current;
        fwrite(current, sizeof(*current), 1, flight_list_file);
        current = current->next;
        free(aux);
    }

    free(list);
}
