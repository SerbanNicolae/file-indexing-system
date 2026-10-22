/* Serban Nicolae 314CA */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "heap.h"
#include "trie.h"
#include "operations.h"

int main(void) {
	FILE *in = fopen("indexare.in", "r");
	FILE *out = fopen("indexare.out", "wt");
	if (in == NULL || out == NULL) {
		return -1;
	}

	list_file *list = create_list();
	trie_cuv *trie = create_trie();

	int n;
	fscanf(in, "%d", &n);

	while (n) {
		char command[20];
		fscanf(in, "%s", command);

		if (strcmp("ADD", command) == 0) {
			add_function(list, trie, in, out);
		} else if (strcmp("DEL", command) == 0) {
			fscanf(in, " %s", command);
			del_function(command, trie, list, out);
		} else if (strcmp("ADDKW", command) == 0) {
			char s1[101], s2[101];
			fscanf(in, " %s", s1);
			fscanf(in, " %s", s2);
			add_kw(s1, s2, list, trie, out);
		} else if (strcmp("DELKW", command) == 0) {
			char s1[101], s2[101];
			fscanf(in, " %s", s1);
			fscanf(in, " %s", s2);
			del_kw(s1, s2, list, trie, out);
		} else if (strcmp("FIND", command) == 0) {
			char s1[100];
			fscanf(in, " %s", s1);
			find_func(s1, trie, out);
		} else if (strcmp("TOPK", command) == 0) {
			char s1[100];
			int k;
			fscanf(in, " %s", s1);
			fscanf(in, "%d", &k);
			topk_func(s1, trie, k, out);
		} else if (strcmp("PRINT", command) == 0) {
			print_trie(trie, out);
		}
		n--;
	}

	free_list(list);
	free_trie(trie);
	fclose(in);
	fclose(out);
	return 0;
}
