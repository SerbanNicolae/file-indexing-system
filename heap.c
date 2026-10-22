/* Serban Nicolae 314CA */
#include "heap.h"

// Allocates a max-heap with default initial capacity
heap_t *create_heap(void) {
	heap_t *heap = calloc(1, sizeof(heap_t));
	heap->size = 0;
	heap->capacity = 10;
	heap->arr = calloc(heap->capacity, sizeof(node_file *));
	return heap;
}

// Helper to sift up an element to restore max-heap property
void __heap_insert_fix(heap_t *heap, int pos) {
	int p = (pos - 1) / 2;

	while (pos > 0) {
		node_file *copil = heap->arr[pos];
		node_file *parinte = heap->arr[p];

		int are_prioritate = 0;

		if (copil->scor > parinte->scor) {
			are_prioritate = 1;
		} else if (copil->scor == parinte->scor && strcmp(copil->file_name, parinte->file_name) < 0) {
			are_prioritate = 1;
		}
		if (!are_prioritate) {
			break;
		}
		heap->arr[pos] = parinte;
		heap->arr[p] = copil;
		pos = p;
		p = (pos - 1) / 2;
	}
}

// Inserts a file pointer into the max-heap
void heap_insert(heap_t *heap, node_file *fisier_nou) {
	if (heap->size == heap->capacity) {
		heap->capacity *= 2;
		heap->arr = (node_file **)realloc(heap->arr, heap->capacity * sizeof(node_file *));
		if (heap->arr == NULL) {
			return;
		}
	}
	heap->arr[heap->size] = fisier_nou;
	__heap_insert_fix(heap, heap->size);
	heap->size++;
}

// Helper to sift down an element to restore max-heap property
void __heap_sift_down(heap_t *heap, int pos) {
	int left, right, largest;
	while (1) {
		left = 2 * pos + 1;
		right = 2 * pos + 2;
		largest = pos;
		if (left < heap->size) {
			if (heap->arr[left]->scor > heap->arr[largest]->scor ||
				(heap->arr[left]->scor == heap->arr[largest]->scor && strcmp(heap->arr[left]->file_name, heap->arr[largest]->file_name) < 0)) {
				largest = left;
			}
		}
		if (right < heap->size) {
			if (heap->arr[right]->scor > heap->arr[largest]->scor ||
				(heap->arr[right]->scor == heap->arr[largest]->scor && strcmp(heap->arr[right]->file_name, heap->arr[largest]->file_name) < 0)) {
				largest = right;
			}
		}
		if (largest != pos) {
			node_file *temp = heap->arr[pos];
			heap->arr[pos] = heap->arr[largest];
			heap->arr[largest] = temp;
			pos = largest;
		} else {
			break;
		}
	}
}

// Removes a specific file from the heap and restores heap property
void heap_remove(heap_t *heap, node_file *tinta) {
	if (heap == NULL || heap->size == 0) return;
	int i;
	for (i = 0; i < heap->size; i++) {
		if (heap->arr[i] == tinta) {
			break;
		}
	}
	if (i == heap->size) return;
	heap->arr[i] = heap->arr[heap->size - 1];
	heap->size--;
	if (i < heap->size) {
		int parent = (i - 1) / 2;
		if (i > 0 && (heap->arr[i]->scor > heap->arr[parent]->scor ||
			(heap->arr[i]->scor == heap->arr[parent]->scor && strcmp(heap->arr[i]->file_name, heap->arr[parent]->file_name) < 0))) {
			__heap_insert_fix(heap, i);
		} else {
			__heap_sift_down(heap, i);
		}
	}
}
