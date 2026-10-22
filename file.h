/* Serban Nicolae 314CA */
#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// File node structure
typedef struct node_file node_file;
struct node_file {
	char file_name[20];
	int scor;
	struct node_file *next, *prev;
	char **key_words;
	int nr_cuv;
};

// Doubly-linked file list structure
typedef struct list_file list_file;
struct list_file {
	node_file *head, *tail;
	int size;
};

// Creates and initializes a new file node
node_file *create_node(void);

// Creates and initializes an empty file list
list_file *create_list(void);

// Frees all memory allocated for the file list
void free_list(list_file *list);

#endif
