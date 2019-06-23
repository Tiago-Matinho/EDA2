#include "air_hash.h"

struct vertice{
    char name[AIRPORT];
    short distance;
    char parent[FLIGHT_CODE];
    struct vertice* next;
    struct dep_list* edges;
};


struct dist_list{
    struct vertice* header;
};


struct visited_list{
    struct vertice* header;
};


struct path{
    struct vertice* header;
};
/*-------------------------------------------------------------------*/

struct vertice* vertice_new(char name[AIRPORT]);
struct dist_list* dist_list_new();
struct visited_list* visited_list_new();
struct path* path_new();

/*-------------------------------------------------------------------*/

bool dist_list_empty(struct dist_list* list);
void dist_list_insert(struct dist_list* list, struct vertice* vertice);
struct vertice* dist_list_get(struct dist_list* list, char name[AIRPORT]);
bool dist_list_exist(struct dist_list* list, char name[AIRPORT]);
struct vertice* dist_list_remove(struct dist_list* list);

/*-------------------------------------------------------------------*/

bool visited_list_empty(struct visited_list* list);
void visited_list_insert(struct visited_list* list, struct vertice* vertice);
bool visited_list_exist(struct visited_list* list, char name[AIRPORT]);
struct vertice* visited_list_get(struct visited_list* list, char name[AIRPORT]);

/*-------------------------------------------------------------------*/

void path_insert(struct path* path, struct vertice* vertice);

/*-------------------------------------------------------------------*/

void dist_list_destroy(struct dist_list* list);
void visited_list_destroy(struct visited_list* list);
void path_list_destroy(struct path* list);
