#include "flight_hash.h"

struct dep_node{
    char name[FLIGHT_CODE];
    struct dep_node* next;
};

struct dep_list{
    struct dep_node* header;
};


/*-------------------------------------------------------------------*/

struct dep_node* dep_node_new(char name[FLIGHT_CODE]);
struct dep_list* dep_list_new();

/*-------------------------------------------------------------------*/

bool dep_list_empty(struct dep_list* list);
void dep_list_insert(struct dep_list* list, struct dep_node* new_node);
void dep_list_remove(struct dep_list* list, char name[FLIGHT_CODE]);
struct dep_node* dep_list_search(struct dep_list* list, char name[FLIGHT_CODE]);

/*-------------------------------------------------------------------*/

void dep_list_read(FILE* flight_list_file, FILE* fly_file, struct fly_hash* fly_hash, struct dep_list* list, int key);
void dep_list_write(FILE* flight_list_file, struct dep_list* list, int key);
void dep_list_destroy(struct dep_list* list);