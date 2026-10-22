/* Serban Nicolae 314CA */
#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "heap.h"

// Trie node structure
typedef struct trie_node trie_node;
struct trie_node {
	heap_t *files_heap;
	int end_of_word;
	trie_node **children;
	int n_children;
};

// Trie structure for storing keywords
typedef struct trie_cuv trie_cuv;
struct trie_cuv {
	trie_node *root;
	int size;
	int alphabet_size;
	char *alphabet;
};

// Allocates and initializes a new trie node
trie_node *create_trie_node(void);

// Allocates and initializes a new trie
trie_cuv *create_trie(void);

// Inserts a keyword and associates a file reference in the trie
void trie_insert(trie_cuv *trie, trie_node *curr, char *key, node_file *file);

// Finds the terminal node corresponding to a keyword
trie_node *nod_terminal(trie_node *curr, char *key);

// Removes a keyword and frees unneeded trie nodes
void trie_remove(trie_cuv *trie, char *key);

// Prints all keywords and their associated files in lexicographical order
void print_trie(trie_cuv *trie, FILE *out);

// Frees all memory allocated for the trie
void free_trie(trie_cuv *trie);

#endif
