/* Serban Nicolae 314CA */
#include "file.h"

// Creates and initializes a new file node
node_file *create_node(void) {
	node_file *node = calloc(1, sizeof(node_file));
	node->next = NULL;
	node->prev = NULL;
	return node;
}

// Creates and initializes an empty file list
list_file *create_list(void) {
	list_file *list = calloc(1, sizeof(list_file));
	return list;
}

// Frees all memory allocated for the file list
void free_list(list_file *list) {
	if (list == NULL) return;
	if (list->head == NULL) {
		free(list);
		return;
	}
	node_file *curr = list->head, *urm;
	while (curr != NULL) {
		urm = curr->next;
		int c = curr->nr_cuv;
		for (int i = 0; i < c; i++) {
			free(curr->key_words[i]);
		}
		free(curr->key_words);
		free(curr);
		curr = urm;
	}
	free(list);
}
