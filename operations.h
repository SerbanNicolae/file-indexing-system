/* Serban Nicolae 314CA */
#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "heap.h"
#include "trie.h"

// Executes the ADD command
void add_function(list_file *list, trie_cuv *trie, FILE *in, FILE *out);

// Executes the DEL command
void del_function(char *name, trie_cuv *trie, list_file *list, FILE *out);

// Executes the ADDKW command
void add_kw(char *id, char *word, list_file *list, trie_cuv *trie, FILE *out);

// Executes the DELKW command
void del_kw(char *id, char *word, list_file *list, trie_cuv *trie, FILE *out);

// Executes the FIND command
void find_func(char *word, trie_cuv *trie, FILE *out);

// Executes the TOPK command
void topk_func(char *word, trie_cuv *trie, int k, FILE *out);

#endif
