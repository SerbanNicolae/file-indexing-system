//Serban Nicolae 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct pt un fisier
typedef struct node_file node_file;
struct node_file {
	char file_name[20];
	int scor;
	struct node_file *next, *prev;
	char **key_words;
	int nr_cuv;

};

//lista pt fisiere
typedef struct list_file list_file;
struct list_file{
	node_file *head, *tail;
	int size;
};

//max-heap pt a sorta fisierele dupa scor
typedef struct heap_t heap_t;
struct heap_t {
    node_file **arr;  
    int size;         
    int capacity;     
};

//un nod din trie 
typedef struct trie_node trie_node;
struct trie_node {
    heap_t* files_heap;
    int end_of_word;
    trie_node** children;
    int n_children;
};

//trie
typedef struct trie_cuv trie_cuv;
struct trie_cuv {

	trie_node *root;
	int size;
	int alphabet_size;
	char *alphabet;

};

//aloca si initializeaza un nod nou pt un fisier si returneaza un pointer la acesta
node_file *create_node() {

	node_file *node = calloc(1, sizeof(node_file));
	node->next = NULL;
	node->prev = NULL;
	return node;

}

//aloca memoria pt lista si returneaza un pointer la ea
list_file *create_list() {
	list_file *list = calloc(1, sizeof(list_file));
	return list;
}

//aloca un max-heap cu o capacitate initiala de 10 si returneaza un pointer la el
heap_t *create_heap() {

	heap_t *heap = calloc(1, sizeof(heap_t));
	heap->size = 0;
	heap->capacity = 10;
	heap->arr = calloc(heap->capacity, sizeof(node_file*));
	return heap;
}

//aloca memorie pt un nod in trie si returneaza un pointer la el
trie_node *create_trie_node() {
	trie_node *node = calloc(1, sizeof(trie_node));
	node->end_of_word = 0;
	node->n_children = 0;
	node->children = calloc(26, sizeof(trie_node*));
	return node;

}

//aloca memorie pt trie ul in sine si returneaza un pointer la acesta
trie_cuv *create_trie() {
	trie_cuv *trie = calloc(1, sizeof(trie_cuv));
	trie->alphabet_size = 26;
	trie->alphabet = malloc(trie->alphabet_size);
	trie->size = 0;	
	trie->root =create_trie_node();
	return trie;
}



//functie ajutatoare care ueca un nod nou adaugat la finalul vectorului pana la pozitia corecta( dupa scor iar daca e egalitate alfabetic) 
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

//insereaza un pointer catre document in heap, daca vect e plin ii dubleaza capcitatea
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


//functia este una recursiva si parcurge litera cu litera un cuvant si la final adauga documentul in heap ul ultimului nod
void trie_insert (trie_cuv * trie, trie_node *curr, char *key, node_file *file ) {

	if(strlen(key) == 0) {
		curr->end_of_word = 1;
		if (curr->files_heap == NULL) {
			curr->files_heap = create_heap();
		}
		heap_insert(curr->files_heap, file);
		return;
	}
	char x = key[0];
	if(curr->children[x - 97] == NULL) {
		curr->children [x-97] = create_trie_node();
		curr->n_children++;	
	}
	trie_insert(trie, curr->children[x-97], key + 1, file);

}
//creeaza un fisier nou verfica daca exsita deja fisierul , si insereaza in trie la finalul cuvantului referinta la fisier pt fiecare cuvant
void add_function(list_file *list, trie_cuv *trie, FILE * in, FILE *out) {
    char name[100];
    fscanf(in, "%s", name);
    node_file *curr = list->head;
    while(curr) {
        if(strcmp(curr->file_name, name) == 0){
            int dummy_scor, dummy_nr;
            fscanf(in, "%d %d", &dummy_scor, &dummy_nr);
            for(int j = 0; j < dummy_nr; j++) {
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
        fscanf(in, "%s", s );
        node->key_words[i++] = calloc(strlen(s) + 1, 1);
        strcpy(node->key_words[i -1], s);
        
        int deja_bagat = 0;
        for(int j = 0; j < i - 1; j++) {
            if(strcmp(node->key_words[j], s) == 0) {
                deja_bagat = 1;
                break; 
            }
        }
        
        if (deja_bagat == 0) {
            trie_insert(trie, trie->root, s, node);
        }
        nr_cuv--;
    }
    
    if(list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    fprintf(out, "OK\n");
}

//functie ajutatoare coboara un nod pana ii gaseste pozitia potrivita in heap
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

//cauta un anumit fisier in heap , il inlocuieste cu ultimul din vector si reface ordinea cu una din functiile ajutatoare
void heap_remove ( heap_t *heap, node_file *tinta) {

	if(heap == NULL || heap->size == 0) return;
	int i;
	for(i = 0; i < heap->size; i++) {
		if(heap->arr[i] == tinta) {
			break;
		}
	}
	if(i == heap->size) return;
	heap->arr[i] = heap->arr[heap->size -1];
	heap->size--;
	if(i < heap->size) {
		int parent = (i - 1) / 2;
		if (i > 0 && (heap->arr[i]->scor > heap->arr[parent]->scor ||
            (heap->arr[i]->scor == heap->arr[parent]->scor && strcmp(heap->arr[i]->file_name, heap->arr[parent]->file_name) < 0))) {
				__heap_insert_fix(heap, i);
			}
			else {
				__heap_sift_down(heap, i);
			}
	}
}

//returneaza daca cyvantul exista nodul final altfel returneaza NULL
trie_node *nod_terminal(trie_node *curr, char *key) {

	if (curr == NULL) {
        return NULL;
    }
	if(key[0] == '\0') {
		if(curr->end_of_word == 1) {
			return curr;
		}
		return NULL;
	}
	int x = key[0] - 97;
	return nod_terminal(curr->children[x], key + 1);
}

//este o functie recursiva , curata heap ul nodului terminal, il marcheaza cu 0 la end_of_word si elibereaza celelalte noduri ale cuvantului care nu au copii
int trie_remove_helper(trie_cuv* trie, trie_node* curr, char* key) {
    if(strlen(key) == 0) {
        if(curr->end_of_word == 1) {
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
    if(curr->children[x - 97] != NULL) {
        int b = trie_remove_helper(trie, curr->children[x - 97], key + 1);
        if(b == 1) {
            free(curr->children[x - 97]->children);
            free(curr->children[x - 97]);
            curr->children[x - 97] = NULL;           
            curr->n_children--;
        }
    }   
    return (curr->end_of_word == 0 && curr->n_children == 0);
}

void trie_remove(trie_cuv* trie, char* key) {

	trie_remove_helper(trie, trie->root, key);
}
//cauta un fisier , daca l gaseste sterge referintele lui din toate heap urile cuvintelor si sterge cuvintele complet daca heap ul lor ramane gol si l eleimina din lista dublu inlantuita
void del_function(char *name, trie_cuv *trie, list_file *list, FILE *out) {

	node_file *curr = list->head;
	while (curr && strcmp(curr->file_name, name) != 0) {
		curr= curr->next;
	}
	if (curr == NULL) {
		fprintf(out, "NOT FOUND\n");
		//printf("n a scris ce trebuie\n");
        return; 
    }

	for(int i = 0; i < curr->nr_cuv; i++) {
		char s[101];
		strcpy(s, curr->key_words[i]);
		trie_node *terminal = nod_terminal(trie->root, s);
		if (terminal != NULL && terminal->files_heap != NULL) {
		heap_remove(terminal->files_heap, curr);
		
			if(terminal->files_heap->size == 0) {
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
	for(int i = 0; i < curr->nr_cuv; i++) {
		free(curr->key_words[i]);
	}
	free(curr->key_words);
	free(curr);
	fprintf(out, "OK\n");
}
//adauga un cuvant nou unui fisier si l adauga in trie 
void add_kw(char *id, char *word, list_file *list, trie_cuv *trie, FILE *out) {
	node_file *curr = list->head;
	while (curr && strcmp(id, curr->file_name)!=0) {
		curr = curr->next;
	}
	if(curr == NULL) {
		fprintf(out, "NOT FOUND\n");
		return;
	}
	int i;
	for(i = 0; i < curr->nr_cuv; i++) {
		if(strcmp(curr->key_words[i], word) == 0) {
			break;
		}
	}
	if (i < curr->nr_cuv) {
		fprintf(out, "OK\n");
		return;
	}
	trie_insert(trie, trie->root, word, curr);
	curr->nr_cuv ++;
	char **aux = realloc(curr->key_words, curr->nr_cuv * sizeof(char *));
	if (aux != NULL) {
		curr->key_words = aux;
	} else {
		printf("realloc a esuat \n");
		return;
	}
	curr->key_words[curr->nr_cuv - 1] = calloc (strlen(word) + 3, 1);
	strcpy(curr->key_words[curr->nr_cuv - 1], word);
	fprintf(out, "OK\n");
}

//sterge un cuvant dintr un fisier existent, se sterge referinta de pe nodul terminal la fisier iar daca heap ul ramane gol se sterge cuvantul din trie 
void del_kw(char *id, char *word, list_file *list, trie_cuv *trie, FILE *out) {
	node_file *curr = list->head;
	while (curr && strcmp(id, curr->file_name)!=0) {
		curr = curr->next;
	}
	if(curr == NULL) {
		fprintf(out, "NOT FOUND\n");
		return;
	}
	int i;
	for(i = 0; i < curr->nr_cuv; i++) {
		if(strcmp(curr->key_words[i], word) == 0) {
			break;
		}
	}
	if (i == curr->nr_cuv) {
		fprintf(out, "OK\n");
		return;
	}

	trie_node *term = nod_terminal(trie->root, word);	
	if(term != NULL && term->files_heap != 0) {
		heap_remove(term->files_heap, curr);
		if(term->files_heap->size == 0) {
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

//gaseste heap ul cuvantului muta fisierele intr un vect temporar si le sorteaza alfabetic
void find_func(char *word, trie_cuv *trie, FILE *out) {
	trie_node *term = nod_terminal(trie->root, word);
	if (term == NULL || term->files_heap == NULL || term->files_heap->size == 0) {
		fprintf(out, "EMPTY\n");
		return;
	}
	node_file **temp = calloc(term->files_heap->size, sizeof(node_file *));
	for(int i = 0; i < term->files_heap->size; i++) {
		temp[i] = term->files_heap->arr[i];
	}
	for(int i =0; i < term->files_heap->size - 1; i++)
		for (int j = i + 1; j < term->files_heap->size; j++) {
			if (strcmp(temp[i]->file_name, temp[j]->file_name) > 0) {
				node_file *temporar = temp[i];
				temp[i] = temp[j];
				temp[j] = temporar;
			}
		}
	fprintf (out, "%d", term->files_heap->size);
	for(int i =0; i < term->files_heap->size; i++){
		fprintf(out, " %s", temp[i]->file_name);
	}
	fprintf(out, "\n");
	free(temp);
}

//gaseste heap ul unui cuvant, ii face o copie (ca sa nu l distruga) si ia primele k eleemnte , extragand mereu radacina si echilibrand
void topk_func(char *word, trie_cuv *trie, int k, FILE *out) {
	trie_node *term = nod_terminal(trie->root, word);
	if (term == NULL || term->files_heap == NULL || term->files_heap->size == 0) {
        fprintf(out, "EMPTY\n");
        return;
    }
	int size  = term->files_heap->size;
	int limita;
	if (k < size) {
		limita = k;
	} else {
		limita = size;
	}
	heap_t *copie = calloc(1, sizeof(heap_t));
	copie->size = size;
	copie->capacity = term->files_heap->capacity;
	node_file **aux = calloc(copie->capacity, sizeof(node_file*));
	if(aux == NULL) {
		printf("callocc failles");
		return;
	}
	copie->arr = aux;
	for(int i = 0; i < size; i++) {
		copie->arr[i] = term->files_heap->arr[i];
	}
	fprintf(out, "%d", limita);
	for (int i = 0; i < limita; i++) {
		fprintf(out, " %s", copie->arr[0]->file_name);
		copie->arr[0] = copie->arr[copie->size - 1];
		copie->size --;
		if(copie->size > 0) {
			__heap_sift_down(copie, 0);
		}
	}
	fprintf(out, "\n");
	free(copie->arr);
	free(copie);
}
//prcurge recursiv dfs a trie ului pentru a genera literele alfabetic, cad gaseste un cuvant sorteaza documentele alfabetic si le afiseaza
void print_trie_helper (trie_node *curr, char *cuv, int poz, int *gasit, FILE *out) {

	if(curr == NULL) {
		return;
	}
	cuv[poz] = '\0';
	if(curr->end_of_word == 1 && curr->files_heap != NULL && curr->files_heap->size >0) {
		*gasit = 1;
		int size = curr->files_heap->size;
		fprintf(out, "%s %d", cuv, size);
		node_file **temp = calloc(size, sizeof(node_file *));
		for(int i = 0; i < size; i++) {
			temp[i] = curr->files_heap->arr[i];
		}
		for(int i = 0; i < size - 1; i++) {
            for (int j = i + 1; j < size; j++) {
                if (strcmp(temp[i]->file_name, temp[j]->file_name) > 0) {
                    node_file *temporar = temp[i];
                    temp[i] = temp[j];
                    temp[j] = temporar;
                }
            }
        }
		for(int i = 0; i < size; i++) {
            fprintf(out, " %s", temp[i]->file_name);
        }
        fprintf(out, "\n");
		free(temp);
	}
	for(int i = 0; i < 26; i++) {
		if(curr->children[i] != NULL) {
			cuv[poz] = i + 'a';
			print_trie_helper(curr->children[i], cuv, poz + 1, gasit,  out);
		}
	}
}

void print_trie(trie_cuv *trie, FILE *out) {
	if(trie == NULL || trie->root == NULL) {
		return;
	}
	char cuv[102];
	int gasit = 0;
	print_trie_helper(trie->root, cuv, 0, &gasit,  out);
	if (gasit == 0) {
        fprintf(out, "EMPTY\n");
    }


}

//parcurge dfs, merge pana la frunze , sterge heap urile si cand se intoarce distruge nodurile 
void free_trie_helper(trie_node *curr) {

	if(curr == NULL) {
		return;
	}

	for(int i = 0; i < 26; i++) {
		if(curr->children[i] != NULL) {
			free_trie_helper(curr->children[i]);
		}
	}

	if(curr->files_heap != NULL) {
		free(curr->files_heap->arr);
		free(curr->files_heap);
	}
	free(curr->children);
	free(curr);
}

void free_trie(trie_cuv *trie) {
	if(trie == NULL) {
		return;
	}
	free_trie_helper(trie->root);
	free(trie->alphabet);
	free(trie);
}
//eliberreaza memoria listei
void free_list(list_file *list) {

	if(list->head == NULL) {
		free(list);
		return;
	}
	node_file *curr = list->head, *urm;
	urm = curr->next;
	int c;
	while (curr != NULL) {
		urm = curr->next;
		c = curr->nr_cuv;
		for(int i = 0; i < c; i++) {
			free(curr->key_words[i]);
		}
		free(curr->key_words);
		free(curr);
		curr = urm;
	}
		free(list);
}

int main() 
{
	FILE *in = fopen("indexare.in", "r");
	FILE *out = fopen("indexare.out", "wt");     
	list_file *list = create_list();
	trie_cuv *trie = create_trie();

int n;
fscanf(in, "%d", &n);
//char f = fgetc(in);
while(n) {
	char command[20];
	fscanf(in, "%s", command);

	if (strcmp("ADD", command) == 0) {
		add_function(list, trie, in, out);
	}
	if (strcmp("DEL", command) == 0) {
		fscanf(in, " %s", command);
		del_function(command, trie, list, out);
	}
	if (strcmp ("ADDKW", command) == 0) {
		char s1[101], s2[101];
		fscanf (in, " %s", s1);
		fscanf (in , " %s", s2);
		add_kw(s1, s2, list, trie, out);
	}
	if(strcmp("DELKW", command) == 0) {
		char s1[101], s2[101];
		fscanf (in, " %s", s1);
		fscanf (in , " %s", s2);
		del_kw(s1, s2, list, trie, out);
	}
	if(strcmp("FIND", command) == 0) {
		char s1[100];
		fscanf (in, " %s", s1);
		find_func(s1, trie, out);
	}
	if(strcmp("TOPK", command) == 0) {
		char s1[100];
		int k;
		fscanf (in, " %s", s1);
		fscanf(in, "%d", &k);
		topk_func(s1, trie, k, out);
	}
	if(strcmp("PRINT", command) == 0) {
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