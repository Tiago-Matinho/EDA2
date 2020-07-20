#include "flight_hash.h"

/*****************************************************************************+
+ Implementac~ao de min_binary_heap baseada no livro                          +
+ escrito por: Cormen T, Leiserson C, Rivest R, Stein C   (2009)              +
+ "Introduction to Algorithms" página 151.	                                  +
******************************************************************************/
struct heap{
    struct vertice* array[HEAP_SIZE];
    int size;
};


struct heap* heap_new();


/*-------------------------------------------------------------------*/



void min_heapify(struct air_hash* hashtable, struct heap* heap, int i);
void build_min_heap(struct air_hash* hashtable, struct heap* heap);
struct vertice* heap_extract_min(struct air_hash* hashtable,
	struct heap* heap);
void heap_decrease_key(struct air_hash* hashtable, struct heap* heap,
	int i, struct vertice* key);
void min_heap_insert(struct air_hash* hashtable, struct heap* heap,
	struct vertice* key);
void change_value(struct air_hash* hashtable, struct heap* heap, int i);
bool heap_empty(struct heap* heap);


/*-------------------------------------------------------------------*/


void min_heap_destroy(struct heap* heap);

void print_heap(struct heap* heap);