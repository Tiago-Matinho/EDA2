#include "min_heap.h"


struct heap* heap_new(){
    struct heap* new = malloc(sizeof(struct heap));

    if(new != NULL){
        for(int i = 0; i < HEAP_SIZE; ++i){
            new->array[i] = NULL;
        }
        new->size = 0;
    }

    return new;
}


/*-------------------------------------------------------------------*/


void swap(struct air_hash* hashtable, struct heap* heap, int i, int j){
    struct vertice* temp;

    struct airport* airport1 = air_search(hashtable, heap->array[i]->name);
    struct airport* airport2 = air_search(hashtable, heap->array[j]->name);
    airport1->pos = j;
    airport2->pos = i;

    temp = heap->array[j];
    heap->array[j] = heap->array[i];
    heap->array[i] = temp;
}


void min_heapify(struct air_hash* hashtable, struct heap* heap, int i){
    int l = LEFT(i);
    int r = RIGHT(i);
    int smalest;

    if(l <= heap->size &&
       heap->array[l]->distance < heap->array[i]->distance)
        smalest = l;

    else
        smalest = i;

    if(r <= heap->size &&
       heap->array[r]->distance < heap->array[smalest]->distance)
        smalest = r;

    if(smalest != i){
        swap(hashtable, heap, i, smalest);
        min_heapify(hashtable, heap, smalest);
    }
}


struct vertice* heap_extract_min(struct air_hash* hashtable, struct heap* heap){
    if(heap->size < 1)
        return NULL;

    struct vertice* min = heap->array[1];
    struct vertice* temp = heap->array[heap->size];

    heap->array[1] = temp;
    heap->array[heap->size] = NULL;

    air_search(hashtable, temp->name)->pos = 1;
    air_search(hashtable, min->name)->pos = -1;

    heap->size--;
    min_heapify(hashtable, heap, 1);
    return min;
}


void heap_decrease_key(struct air_hash* hashtable, struct heap* heap, int i, struct vertice* key){

    heap->array[i] = key;
    air_search(hashtable, key->name)->pos = i;

    while(i > 1 &&
          heap->array[PARENT(i)]->distance > heap->array[i]->distance){

        swap(hashtable, heap, i, PARENT(i));
        i = PARENT(i);
    }
}


void min_heap_insert(struct air_hash* hashtable, struct heap* heap, struct vertice* key){
    if(heap->size == 0){
        heap->size++;
        heap->array[heap->size] = key;
        air_search(hashtable, key->name)->pos = heap->size;
        return;
    }
    heap->size++;
    air_search(hashtable, key->name)->pos = heap->size;
    heap_decrease_key(hashtable, heap, heap->size, key);
}


bool heap_empty(struct heap* heap){
    return heap->size == 0 ? true : false;
}


void change_value(struct air_hash* hashtable, struct heap* heap, int i){
    while(i > 1 &&
          heap->array[PARENT(i)]->distance > heap->array[i]->distance){

        swap(hashtable, heap, i, PARENT(i));
        i = PARENT(i);
    }
}


void min_heap_destroy(struct heap* heap){
    for(int i = 0; i < HEAP_SIZE; i++){
        heap->array[i] = NULL;
    }
    free(heap);
}