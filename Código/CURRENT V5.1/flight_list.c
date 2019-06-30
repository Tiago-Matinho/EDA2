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
    new_node->next = list->header;
    list->header = new_node;
}


void flight_list_remove(struct flight_list* list, char name[FLIGHT_CODE]){
    if(flight_list_empty(list))
        return;

    struct list_node* current = list->header;
    struct list_node* aux;

    //apenas um elemento
    if(current->next == NULL && strcmp(current->element->name, name) == 0){
        free(current);
        list->header = NULL;
        return;
    }

    //elemento 'e o primeiro da lista
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

        current = current->next;
    }

    printf("Erro a remover da lista\n");
}


struct list_node* flight_list_pop(struct flight_list* list){
    struct list_node* temp = list->header;
    list->header = temp->next;
    return temp;
}


/*---------------------------------------------------------------------------*/


void flight_list_destroy(struct flight_list* list){
    if(list == NULL)
        return;

    struct list_node* current = list->header;
    struct list_node* aux = current;

    while(current != NULL){
        current = current->next;
        free(aux);
        aux = current;
    }

    free(list);
}