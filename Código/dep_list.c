#include "dep_list.h"


struct dep_node* dep_node_new(char name[FLIGHT_CODE], char dest[AIRPORT],
                              short dep_time, short duration)
{
    struct dep_node* new_node = malloc(sizeof(struct dep_node));

    if(new_node != NULL){
        strcpy(new_node->name, name);
        strcpy(new_node->dest, dest);
        new_node->dep_time = dep_time;
        new_node->duration = duration;
        new_node->next = NULL;
    }
    return new_node;
}


struct dep_list* dep_list_new(){
    struct dep_list* new_list = malloc(sizeof(struct dep_list));

    if(new_list != NULL)
        new_list->header = NULL;

    return new_list;
}


/*-------------------------------------------------------------------*/


bool dep_list_empty(struct dep_list* list){
    return list->header == NULL;
}

/*-------------------------------------------------------------------*/

void dep_list_insert(struct dep_list* list, struct dep_node* new_node){

    //first insertion
    if(dep_list_empty(list)){
        list->header = new_node;
        return;
    }

    struct dep_node* current = list->header;

    //insert on head
    if(new_node->dep_time < current->dep_time){
        new_node->next = current;
        list->header = new_node;
        return;
    }

    while(current->next != NULL){

        if(new_node->dep_time < current->next->dep_time){
            new_node->next = current->next;
            current->next = new_node;
            return;
        }

        current = current->next;
    }

    current->next = new_node;
}


void dep_list_remove(struct dep_list* list, char name[FLIGHT_CODE]){
    //nothing to remove
    if(dep_list_empty(list))
        return;

    struct dep_node* current = list->header;

    //one element list
    if(current->next == NULL){
        free(current);
        list->header = NULL;
        return;
    }

    while(current->next != NULL){

        if(strcmp(current->next->name, name) == 0){
            current->next = current->next->next;
            free(current->next);
            return;
        }

        current = current->next;
    }
}


struct dep_node* dep_list_search(struct dep_list* list, char name[FLIGHT_CODE]){
    if(dep_list_empty(list))
        return NULL;

    struct dep_node* current = list->header;

    while(current->next != NULL){

        if(strcmp(current->name, name) == 0)
            return current;

        current = current->next;
    }
    return NULL;
}


struct dep_node* flights_after(struct dep_list* list, short dep_time){
    if(dep_list_empty(list))
        return NULL;

    struct dep_node* current = list->header;


    while(current != NULL){

        if(dep_time <= current->dep_time)
            return current;

        current = current->next;
    }

    return NULL;
}


/*-------------------------------------------------------------------*/

void dep_list_insert2(struct dep_list* list, struct dep_node* new_node){

    //first insertion
    if(dep_list_empty(list)){
        list->header = new_node;
        return;
    }

    struct dep_node* current = list->header;

    //insert on head
    if(strcmp(new_node->dest, current->dest) < 0){
        new_node->next = current;
        list->header = new_node;
        return;
    }

    while(current->next != NULL){

        if(strcmp(new_node->dest, current->dest) < 0){
            new_node->next = current->next;
            current->next = new_node;
            return;
        }

        current = current->next;
    }

    current->next = new_node;
}


void dep_list_remove2(struct dep_list* list, char name[FLIGHT_CODE]){
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

        if(strcmp(name, current->next->name) == 0){
            aux = current->next;
            current->next = aux->next;
            free(aux);
            return;
        }

        if(strcmp(name, current->next->name) < 0)
            return;

        current = current->next;
    }
}


struct dep_node* dep_list_search2(struct dep_list* list, char name[FLIGHT_CODE]){
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
