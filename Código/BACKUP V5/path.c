#include "path.h"


void path_insert(struct flight_list* list, struct list_node* node){
    node->next = list->header;
    list->header = node;
}

struct list_node* path_remove(struct flight_list* list){
    struct list_node* temp = list->header;
    list->header = temp->next;
    return temp;
}
