/* Serban Nicolae 314CA */
#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

// Max-heap structure for sorting files by relevance score
typedef struct heap_t heap_t;
struct heap_t {
	node_file **arr;
	int size;
	int capacity;
};

// Allocates a max-heap with default initial capacity
heap_t *create_heap(void);

// Inserts a file pointer into the max-heap
void heap_insert(heap_t *heap, node_file *fisier_nou);

// Removes a specific file from the heap and restores heap property
void heap_remove(heap_t *heap, node_file *tinta);

// Helper to sift up an element to restore max-heap property
void __heap_insert_fix(heap_t *heap, int pos);

// Helper to sift down an element to restore max-heap property
void __heap_sift_down(heap_t *heap, int pos);

#endif
