/* Serban Nicolae 314CA */
#include "operations.h"

// Executes the ADD command
void add_function(list_file *list, trie_cuv *trie, FILE *in, FILE *out) {
	char name[100];
	fscanf(in, "%s", name);
	node_file *curr = list->head;
	while (curr) {
		if (strcmp(curr->file_name, name) == 0) {
			int dummy_scor, dummy_nr;
			fscanf(in, "%d %d", &dummy_scor, &dummy_nr);
			for (int j = 0; j < dummy_nr; j++) {
				char dummy_s[101];
				fscanf(in, "%s", dummy_s);
			}
			fprintf(out, "EXISTS\n");
			return;
		}
		curr = curr->next;
	}

	node_file *node = create_node();
	strcpy(node->file_name, name);
	fscanf(in, "%d", &node->scor);
	int nr_cuv;
	fscanf(in, "%d", &nr_cuv);
	node->nr_cuv = nr_cuv;
	node->key_words = calloc(nr_cuv, sizeof(char *));
	int i = 0;

	while (nr_cuv) {
		char s[101];
		fscanf(in, "%s", s);
		node->key_words[i++] = calloc(strlen(s) + 1, 1);
		strcpy(node->key_words[i - 1], s);

		int deja_bagat = 0;
		for (int j = 0; j < i - 1; j++) {
			if (strcmp(node->key_words[j], s) == 0) {
				deja_bagat = 1;
				break;
		}
		}

		if (deja_bagat == 0) {
			trie_insert(trie, trie->root, s, node);
		}
		nr_cuv--;
	}

	if (list->head == NULL) {
		list->head = node;
		list->tail = node;
	} else {
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
	fprintf(out, "OK\n");
}

// Executes the DEL command
void del_function(char *name, trie_cuv *trie, list_file *list, FILE *out) {
	node_file *curr = list->head;
	while (curr && strcmp(curr->file_name, name) != 0) {
		curr = curr->next;
	}
	if (curr == NULL) {
		fprintf(out, "NOT FOUND\n");
		return;
	}

	for (int i = 0; i < curr->nr_cuv; i++) {
		char s[101];
		strcpy(s, curr->key_words[i]);
		trie_node *terminal = nod_terminal(trie->root, s);
		if (terminal != NULL && terminal->files_heap != NULL) {
			heap_remove(terminal->files_heap, curr);
			if (terminal->files_heap->size == 0) {
				trie_remove(trie, s);
			}
		}
	}
	if (curr->prev != NULL) {
		curr->prev->next = curr->next;
	} else {
		list->head = curr->next;
	}
	if (curr->next != NULL) {
		curr->next->prev = curr->prev;
	} else {
		list->tail = curr->prev;
	}
	for (int i = 0; i < curr->nr_cuv; i++) {
		free(curr->key_words[i]);
	}
	free(curr->key_words);
	free(curr);
	fprintf(out, "OK\n");
}

// Executes the ADDKW command
void add_kw(char *id, char *word, list_file *list, trie_cuv *trie, FILE *out) {
	node_file *curr = list->head;
	while (curr && strcmp(id, curr->file_name) != 0) {
		curr = curr->next;
	}
	if (curr == NULL) {
		fprintf(out, "NOT FOUND\n");
		return;
	}
	int i;
	for (i = 0; i < curr->nr_cuv; i++) {
		if (strcmp(curr->key_words[i], word) == 0) {
			break;
		}
	}
	if (i < curr->nr_cuv) {
		fprintf(out, "OK\n");
		return;
	}
	trie_insert(trie, trie->root, word, curr);
	curr->nr_cuv++;
	char **aux = realloc(curr->key_words, curr->nr_cuv * sizeof(char *));
	if (aux != NULL) {
		curr->key_words = aux;
	} else {
		printf("realloc a esuat \n");
		return;
	}
	curr->key_words[curr->nr_cuv - 1] = calloc(strlen(word) + 3, 1);
	strcpy(curr->key_words[curr->nr_cuv - 1], word);
	fprintf(out, "OK\n");
}

// Executes the DELKW command
void del_kw(char *id, char *word, list_file *list, trie_cuv *trie, FILE *out) {
	node_file *curr = list->head;
	while (curr && strcmp(id, curr->file_name) != 0) {
		curr = curr->next;
	}
	if (curr == NULL) {
		fprintf(out, "NOT FOUND\n");
		return;
	}
	int i;
	for (i = 0; i < curr->nr_cuv; i++) {
		if (strcmp(curr->key_words[i], word) == 0) {
			break;
		}
	}
	if (i == curr->nr_cuv) {
		fprintf(out, "OK\n");
		return;
	}

	trie_node *term = nod_terminal(trie->root, word);
	if (term != NULL && term->files_heap != 0) {
		heap_remove(term->files_heap, curr);
		if (term->files_heap->size == 0) {
			trie_remove(trie, word);
		}
	}

	free(curr->key_words[i]);
	for (int j = i; j < curr->nr_cuv - 1; j++) {
		curr->key_words[j] = curr->key_words[j + 1];
	}
	curr->nr_cuv--;
	if (curr->nr_cuv > 0) {
		curr->key_words = realloc(curr->key_words, curr->nr_cuv * sizeof(char *));
	} else {
		if (curr->prev != NULL) {
			curr->prev->next = curr->next;
		} else {
			list->head = curr->next;
		}
		if (curr->next != NULL) {
			curr->next->prev = curr->prev;
		} else {
			list->tail = curr->prev;
		}
		free(curr->key_words);
		free(curr);
	}
	fprintf(out, "OK\n");
}

// Executes the FIND command
void find_func(char *word, trie_cuv *trie, FILE *out) {
	trie_node *term = nod_terminal(trie->root, word);
	if (term == NULL || term->files_heap == NULL || term->files_heap->size == 0) {
		fprintf(out, "EMPTY\n");
		return;
	}
	node_file **temp = calloc(term->files_heap->size, sizeof(node_file *));
	for (int i = 0; i < term->files_heap->size; i++) {
		temp[i] = term->files_heap->arr[i];
	}
	for (int i = 0; i < term->files_heap->size - 1; i++) {
		for (int j = i + 1; j < term->files_heap->size; j++) {
			if (strcmp(temp[i]->file_name, temp[j]->file_name) > 0) {
				node_file *temporar = temp[i];
				temp[i] = temp[j];
				temp[j] = temporar;
			}
		}
	}
	fprintf(out, "%d", term->files_heap->size);
	for (int i = 0; i < term->files_heap->size; i++) {
		fprintf(out, " %s", temp[i]->file_name);
	}
	fprintf(out, "\n");
	free(temp);
}

// Executes the TOPK command
void topk_func(char *word, trie_cuv *trie, int k, FILE *out) {
	trie_node *term = nod_terminal(trie->root, word);
	if (term == NULL || term->files_heap == NULL || term->files_heap->size == 0) {
		fprintf(out, "EMPTY\n");
		return;
	}
	int size = term->files_heap->size;
	int limita;
	if (k < size) {
		limita = k;
	} else {
		limita = size;
	}
	heap_t *copie = calloc(1, sizeof(heap_t));
	copie->size = size;
	copie->capacity = term->files_heap->capacity;
	node_file **aux = calloc(copie->capacity, sizeof(node_file *));
	if (aux == NULL) {
		printf("callocc failles");
		return;
	}
	copie->arr = aux;
	for (int i = 0; i < size; i++) {
		copie->arr[i] = term->files_heap->arr[i];
	}
	fprintf(out, "%d", limita);
	for (int i = 0; i < limita; i++) {
		fprintf(out, " %s", copie->arr[0]->file_name);
		copie->arr[0] = copie->arr[copie->size - 1];
		copie->size--;
		if (copie->size > 0) {
			__heap_sift_down(copie, 0);
		}
	}
	fprintf(out, "\n");
	free(copie->arr);
	free(copie);
}
