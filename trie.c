/* Serban Nicolae 314CA */
#include "trie.h"

// Allocates and initializes a new trie node
trie_node *create_trie_node(void) {
	trie_node *node = calloc(1, sizeof(trie_node));
	node->end_of_word = 0;
	node->n_children = 0;
	node->children = calloc(26, sizeof(trie_node *));
	return node;
}

// Allocates and initializes a new trie
trie_cuv *create_trie(void) {
	trie_cuv *trie = calloc(1, sizeof(trie_cuv));
	trie->alphabet_size = 26;
	trie->alphabet = malloc(trie->alphabet_size);
	trie->size = 0;
	trie->root = create_trie_node();
	return trie;
}

// Inserts a keyword and associates a file reference in the trie
void trie_insert(trie_cuv *trie, trie_node *curr, char *key, node_file *file) {
	if (strlen(key) == 0) {
		curr->end_of_word = 1;
		if (curr->files_heap == NULL) {
			curr->files_heap = create_heap();
		}
		heap_insert(curr->files_heap, file);
		return;
	}
	char x = key[0];
	if (curr->children[x - 97] == NULL) {
		curr->children[x - 97] = create_trie_node();
		curr->n_children++;
	}
	trie_insert(trie, curr->children[x - 97], key + 1, file);
}

// Finds the terminal node corresponding to a keyword
trie_node *nod_terminal(trie_node *curr, char *key) {
	if (curr == NULL) {
		return NULL;
	}
	if (key[0] == '\0') {
		if (curr->end_of_word == 1) {
			return curr;
		}
		return NULL;
	}
	int x = key[0] - 97;
	return nod_terminal(curr->children[x], key + 1);
}

// Recursively frees unneeded trie nodes after keyword removal
static int trie_remove_helper(trie_cuv *trie, trie_node *curr, char *key) {
	if (strlen(key) == 0) {
		if (curr->end_of_word == 1) {
			curr->end_of_word = 0;
			if (curr->files_heap != NULL) {
				free(curr->files_heap->arr);
				free(curr->files_heap);
				curr->files_heap = NULL;
			}
		}
		return curr->n_children == 0;
	}
	char x = key[0];
	if (curr->children[x - 97] != NULL) {
		int b = trie_remove_helper(trie, curr->children[x - 97], key + 1);
		if (b == 1) {
			free(curr->children[x - 97]->children);
			free(curr->children[x - 97]);
			curr->children[x - 97] = NULL;
			curr->n_children--;
		}
	}
	return (curr->end_of_word == 0 && curr->n_children == 0);
}

// Removes a keyword and frees unneeded trie nodes
void trie_remove(trie_cuv *trie, char *key) {
	trie_remove_helper(trie, trie->root, key);
}

// Recursive helper for printing trie words in lexicographical order
static void print_trie_helper(trie_node *curr, char *cuv, int poz, int *gasit, FILE *out) {
	if (curr == NULL) {
		return;
	}
	cuv[poz] = '\0';
	if (curr->end_of_word == 1 && curr->files_heap != NULL && curr->files_heap->size > 0) {
		*gasit = 1;
		int size = curr->files_heap->size;
		fprintf(out, "%s %d", cuv, size);
		node_file **temp = calloc(size, sizeof(node_file *));
		for (int i = 0; i < size; i++) {
			temp[i] = curr->files_heap->arr[i];
		}
		for (int i = 0; i < size - 1; i++) {
			for (int j = i + 1; j < size; j++) {
				if (strcmp(temp[i]->file_name, temp[j]->file_name) > 0) {
					node_file *temporar = temp[i];
					temp[i] = temp[j];
					temp[j] = temporar;
				}
			}
		}
		for (int i = 0; i < size; i++) {
			fprintf(out, " %s", temp[i]->file_name);
		}
		fprintf(out, "\n");
		free(temp);
	}
	for (int i = 0; i < 26; i++) {
		if (curr->children[i] != NULL) {
			cuv[poz] = i + 'a';
			print_trie_helper(curr->children[i], cuv, poz + 1, gasit, out);
		}
	}
}

// Prints all keywords and their associated files in lexicographical order
void print_trie(trie_cuv *trie, FILE *out) {
	if (trie == NULL || trie->root == NULL) {
		return;
	}
	char cuv[102];
	int gasit = 0;
	print_trie_helper(trie->root, cuv, 0, &gasit, out);
	if (gasit == 0) {
		fprintf(out, "EMPTY\n");
	}
}

// Recursive helper for freeing trie nodes
static void free_trie_helper(trie_node *curr) {
	if (curr == NULL) {
		return;
	}
	for (int i = 0; i < 26; i++) {
		if (curr->children[i] != NULL) {
			free_trie_helper(curr->children[i]);
		}
	}
	if (curr->files_heap != NULL) {
		free(curr->files_heap->arr);
		free(curr->files_heap);
	}
	free(curr->children);
	free(curr);
}

// Frees all memory allocated for the trie
void free_trie(trie_cuv *trie) {
	if (trie == NULL) {
		return;
	}
	free_trie_helper(trie->root);
	free(trie->alphabet);
	free(trie);
}
