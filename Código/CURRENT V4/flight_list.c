#include "flight_list.h"


struct list_node* list_node_new(struct flight* flight){
    struct list_node* new = malloc(sizeof(struct list_node));

    if(new != NULL){
        new->element = flight;
        new->next = NULL;
    }
    return new;
}


struct flight_list* flight_list_new(){
    struct flight_list* new = malloc(sizeof(struct flight_list));

    if(new != NULL)
        new->header = NULL;

    return new;
}


/*---------------------------------------------------------------------------*/


bool flight_list_empty(struct flight_list* list){
    return list->header == NULL ? true : false;
}


void flight_list_insert(struct flight_list* list, struct list_node* new_node){
    //first insertion
    if(flight_list_empty(list)){
        list->header = new_node;
        return;
    }

    struct list_node* current = list->header;

    //insert on head
    if(strcmp(current->element->name, new_node->element->name) >= 0){
        new_node->next = current;
        list->header = new_node;
        return;
    }

    while(current->next != NULL){
        if(strcmp(current->element->name, new_node->element->name) >= 0)
            break;

        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}


void flight_list_remove(struct flight_list* list, char name[FLIGHT_CODE]){
    //nothing to remove
    if(flight_list_empty(list))
        return;

    struct list_node* current = list->header;
    struct list_node* aux;

    //one element list
    if(current->next == NULL && strcmp(current->element->name, name) == 0){
        free(current);
        list->header = NULL;
        return;
    }

    if(strcmp(name, current->element->name) == 0){
        list->header = current->next;
        free(current);
        return;
    }

    while(current->next != NULL){

        if(strcmp(current->next->element->name, name) == 0){
            aux = current->next;
            current->next = aux->next;
            free(aux);
            return;
        }

        if(strcmp(current->next->element->name, name) > 0)
            return;

        current = current->next;
    }
}


struct list_node* flight_list_search(struct flight_list* list, char flight[FLIGHT_CODE]){
    if(flight_list_empty(list))
        return NULL;

    struct list_node* current = list->header;

    while(current->next != NULL){

        if(strcmp(current->next->element->name, flight) == 0)
            return current->next;

        if(strcmp(current->next->element->name, flight) > 0)
            return NULL;

        current = current->next;
    }
    return NULL;}


/*---------------------------------------------------------------------------*/


void flight_list_destroy(struct flight_list* list){
    if(list == NULL)
        return;

    struct list_node* current = list->header;
    struct list_node* aux = current;

    while(current->next != NULL){
        free(aux);
        current = current->next;
        aux = current;
    }

    //free(aux);
    free(list);
}