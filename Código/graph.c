#include "graph.h"

struct vertice* vertice_new(char name[AIRPORT]){
    struct vertice* new_vertice = malloc(sizeof(struct vertice));

    if(new_vertice != NULL){
        strcpy(new_vertice->name, name);
        new_vertice->distance = 1441;
        memset(new_vertice->parent, '\0', FLIGHT_CODE);
        new_vertice->next = NULL;
        new_vertice->edges = NULL;
    }
    return new_vertice;
}


struct dist_list* dist_list_new(){
    struct dist_list* new_list = malloc(sizeof(struct dist_list));

    if(new_list != NULL)
        new_list->header = NULL;

    return new_list;
}


struct visited_list* visited_list_new(){
    struct visited_list* new_list = malloc(sizeof(struct visited_list));

    if(new_list != NULL)
        new_list->header = NULL;

    return new_list;
}


struct path* path_new(){
    struct path* new_list = malloc(sizeof(struct path));

    if(new_list != NULL)
        new_list->header = NULL;

    return new_list;
}

/*-------------------------------------------------------------------*/

bool dist_list_empty(struct dist_list* list){
    return list->header == NULL;
}


void dist_list_insert(struct dist_list* list, struct vertice* vertice){
    //first insertion
    if(dist_list_empty(list)){
        list->header = vertice;
        return;
    }

    struct vertice* current = list->header;

    //insert on head
    if(vertice->distance < current->distance){
        vertice->next = current;
        list->header = vertice;
        return;
    }

    while(current->next != NULL){

        if(vertice->distance < current->next->distance){
            vertice->next = current->next;
            current->next = vertice;
            return;
        }

        current = current->next;
    }

    current->next = vertice;
}


struct vertice* dist_list_get(struct dist_list* list, char name[AIRPORT]){
    //nothing to remove
    if(dist_list_empty(list))
        return NULL;

    struct vertice* current = list->header;

    //one element list
    if(current->next == NULL){
        list->header = NULL;
        return current;
    }

    while(current->next != NULL){

        if(strcmp(current->next->name, name) == 0){
            current->next = current->next->next;
            return current;
        }

        current = current->next;
    }

    return NULL;
}


struct vertice* dist_list_remove(struct dist_list* list){
    if(dist_list_empty(list))
        return NULL;

    struct vertice* current = list->header;
    list->header = current->next;
    return current;
}

/*-------------------------------------------------------------------*/

bool visited_list_empty(struct visited_list* list){
    return list->header == NULL;
}


void visited_list_insert(struct visited_list* list, struct vertice* vertice){
    //first insertion
    if(visited_list_empty(list)){
        list->header = vertice;
        return;
    }

    struct vertice* current = list->header;

    //insert on head
    if(strcmp(vertice->name, current->name) < 0){
        vertice->next = current;
        list->header = vertice;
        return;
    }

    while(current->next != NULL){

        if(strcmp(vertice->name, current->name) < 0){
            vertice->next = current->next;
            current->next = vertice;
            return;
        }

        current = current->next;
    }

    current->next = vertice;
}


bool visited_list_exist(struct visited_list* list, char name[AIRPORT]){
    //first insertion
    if(visited_list_empty(list))
        return false;

    struct vertice* current = list->header;

    while(current != NULL){

        if(strcmp(name, current->name) == 0)
            return true;

        if(strcmp(name, current->name) > 0)
            break;

        current = current->next;
    }

    return false;
}


struct vertice* visited_list_get(struct visited_list* list, char name[AIRPORT]){
    if(visited_list_empty(list))
        return NULL;

    struct vertice* current = list->header;
    struct vertice* temp;

    if(current->next == NULL && strcmp(current->name, name) == 0){
        list->header = NULL;
        return current;
    }

    while(current->next != NULL){

        if(strcmp(name, current->next->name) == 0){
            temp = current->next;
            current->next = current->next->next;
            return temp;
        }

        if(strcmp(name, current->next->name) < 0)
            break;

        current = current->next;
    }

    return NULL;
}

/*-------------------------------------------------------------------*/

void path_insert(struct path* path, struct vertice* vertice){
    vertice->next = path->header;
    path->header = vertice;
}

/*-------------------------------------------------------------------*/

void dist_list_destroy(struct dist_list* list){
    struct vertice* current = list->header;
    struct vertice* aux;

    while(current != NULL){
        aux = current;
        current = current->next;

        if(aux->edges != NULL)
            free(aux->edges);

        free(aux);
    }

    free(list);
}


void visited_list_destroy(struct visited_list* list){
    struct vertice* current = list->header;
    struct vertice* aux;

    while(current != NULL){
        aux = current;
        current = current->next;

        if(aux->edges != NULL)
            free(aux->edges);

        free(aux);
    }

    free(list);
}


void path_destroy(struct path* list){
    struct vertice* current = list->header;
    struct vertice* aux;

    while(current != NULL){
        aux = current;
        current = current->next;

        if(aux->edges != NULL)
            free(aux->edges);

        free(aux);
    }

    free(list);
}