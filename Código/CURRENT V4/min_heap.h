#include "flight_hash.h"


struct vertice{
    char name[AIRPORT];
    char flight[FLIGHT_CODE];
    struct vertice* parent;
    bool visited;
    int distance;
};


struct heap{
    struct vertice* array[HEAP_SIZE];
    struct vertice* visited[HEAP_SIZE];
    int size;
    int visited_size;
};


struct vertice* vertice_new(char name[AIRPORT]);
struct heap* heap_new();


/*-------------------------------------------------------------------*/



void min_heapify(struct air_hash* hashtable, struct heap* heap, int i);
void build_min_heap(struct air_hash* hashtable, struct heap* heap);
struct vertice* heap_extract_min(struct air_hash* hashtable, struct heap* heap);
void heap_decrease_key(struct air_hash* hashtable, struct heap* heap, int i, struct vertice* key);
void min_heap_insert(struct air_hash* hashtable, struct heap* heap, struct vertice* key);
void min_change_value(struct air_hash* hashtable, struct heap* heap, int i);
bool heap_empty(struct heap* heap);


/*-------------------------------------------------------------------*/


void min_heap_destroy(struct heap* heap);