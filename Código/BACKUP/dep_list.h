#include "defines.h"

struct dep_node{
    char name[FLIGHT_CODE];
    char dest[AIRPORT];
    short dep_time;
    short duration;
    struct dep_node* next;
};

struct dep_list{
    struct dep_node* header;
};


/*-------------------------------------------------------------------*/

struct dep_node* dep_node_new(char name[FLIGHT_CODE], char dest[AIRPORT],
                              short dep_time, short duration);
struct dep_list* dep_list_new();

bool dep_list_empty(struct dep_list* list);
void dep_list_insert(struct dep_list* list, struct dep_node* new_node);
void dep_list_remove(struct dep_list* list, char name[FLIGHT_CODE]);
struct dep_node* dep_list_search(struct dep_list* list, char name[FLIGHT_CODE]);


void dep_list_read();
void dep_list_write();
void dep_list_destroy();