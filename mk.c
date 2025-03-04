// Copyright @lucabotez

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

#define WORD_SIZE 256

// implementarea din laborator a structurii de date

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
	int end_of_word;

	int frequency;

	trie_node_t **children;
	int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
	trie_node_t *root;

	int size;

	int data_size;

	int alphabet_size;
};

trie_node_t *trie_create_node(trie_t *trie)
{
	trie_node_t *node = malloc(sizeof(trie_node_t));
	DIE(!node, "node malloc");

	node->end_of_word = 0;
	node->frequency = 0;
	node->n_children = 0;
	node->children = calloc(trie->alphabet_size, sizeof(trie_node_t *));

	return node;
}

trie_t *trie_create(int data_size, int alphabet_size)
{
	trie_t *trie = malloc(sizeof(trie_t));
	DIE(!trie, "trie malloc");

	trie->size = 0;
	trie->data_size = data_size;
	trie->alphabet_size = alphabet_size;

	trie->root = trie_create_node(trie);
	return trie;
}

void trie_insert(trie_t *trie, char *key)
{
	if (key[0] == '\0') {
		trie->root->end_of_word = 1;
		return;
	}

	trie_node_t *current = trie->root;
	int i = 0;

	while (key[i] != '\0') {
		int letter = key[i] - 'a';

		if (!current->children[letter]) {
			current->children[letter] = trie_create_node(trie);
			current->n_children++;
		}

		current = current->children[letter];
		i++;
	}

	if (current->end_of_word) {
		current->frequency++;
	} else {
		current->end_of_word = 1;
		current->frequency = 1;
	}

	trie->size++;
}

void trie_free_subtrie(trie_t *trie, trie_node_t *node)
{
	if (!node)
		return;

	for (int i = 0; i < trie->alphabet_size && node->n_children; ++i) {
		if (!node->children[i])
			continue;

		trie_free_subtrie(trie, node->children[i]);
		node->n_children--;
		node->children[i] = NULL;
	}

	free(node->children);
	free(node);
}

void trie_free(trie_t **p_trie)
{
	trie_free_subtrie(*p_trie, (*p_trie)->root);
	free(*p_trie);
}

void trie_remove(trie_t *trie, char *key)
{
	if (key[0] == '\0') {
		trie->root->end_of_word = 0;
		trie->size--;
		return;
	}

	trie_node_t *current = trie->root;
	trie_node_t *parent = trie->root;
	int parent_letter = (key[0] - 'a');
	int i = 0;

	while (key[i] != '\0') {
		int letter = key[i] - 'a';

		if (!current->children[letter])
			return;

		if (current->n_children > 1 || current->end_of_word) {
			parent = current;
			parent_letter = letter;
		}

		current = current->children[letter];

		i++;
	}

	current->end_of_word = 0;
	current->frequency = 0;
	if (!current->n_children) {
		trie_free_subtrie(trie, parent->children[parent_letter]);
		parent->children[parent_letter] = NULL;
		parent->n_children--;
	}

	trie->size--;
}

// functia calculeaza si returneaza numarul de litere diferite dintre doua
// cuvinte de acceasi dimensiune word1 si word2
int difference(char *word1, char *word2)
{
	int num = 0;
	for (int i = 0; i < strlen(word1); i++)
		if (word1[i] != word2[i])
			num++;

	return num;
}

// functia parcurge intregul trie, verificand pentru fiecare cuvant daca difera
// in cel mult num litere si este de aceeasi lungime cu word, afisandu-le pe
// cele ce respecta conditiile in ordine lexicografica (acestea fiind retinute
// in new)
void autocorrect(trie_node_t *node, char *new, int length, char *word,
				 int num, int *check)
{
	if (node->end_of_word) {
		new[length] = '\0';

		// verificam daca sunt respectate conditiile
		if (length == strlen(word) && difference(word, new) <= num) {
			printf("%s\n", new);

			// tinem minte daca am afisat macar un cuvant
			*check = 1;
		}
	}

	// parcurgere recursiva prin trie
	for (int i = 0; i < 26; i++)
		if (node->children[i]) {
			new[length] = i + 'a';

			autocorrect(node->children[i], new, length + 1,
						word, num, check);
		}
}

// functia gaseste si afiseaza primul cuvant ce are prefixul corespunzator
// (cel mai mic lexicografic)
void autocomplete_1(trie_node_t *node, char *word, int length)
{
	// formam cuvantul
	while (node->end_of_word == 0)
		for (int i = 0; i < 26; i++)
			if (node->children[i]) {
				word[length] = i + 'a';
				length++;

				// in momentul gasirii primei litere salvate ne mutam pe nodul
				// corespunzator
				node = node->children[i];
				break;
			}

	word[length] = '\0';
	printf("%s\n", word);
}

// functia gaseste cel mai scurt cuvant ce are prefixul corespunzator
void autocomplete_2(trie_node_t *node, char *prefix, char *shortest,
					int length, int *first)
{
	if (node->end_of_word) {
		prefix[length] = '\0';

		// pt primul cuvant gasit nu mai verificam lungimea
		if (*first == 1) {
			*first = 0;
			strcpy(shortest, prefix);
		} else if (length < strlen(shortest)) {
			strcpy(shortest, prefix);
		}
	}

	// parcurgere recursiva prin trie
	for (int i = 0; i < 26; i++)
		if (node->children[i]) {
			prefix[length] = i + 'a';
			autocomplete_2(node->children[i], prefix, shortest,
						   length + 1, first);
		}
}

// functia gaseste cel mai frecvent folosit cuvant ce are prefixul
// corespunzator
void autocomplete_3(trie_node_t *node, char *prefix, char *freq,
					int *prev_freq, int length)
{
	if (node->end_of_word) {
		prefix[length] = '\0';

		// comparam frecventa cuvantului gasit cu cea mai buna frecventa
		// intalnita pana atunci
		if ((*prev_freq) < node->frequency) {
			strcpy(freq, prefix);
			*prev_freq = node->frequency;
		}
	}

	// parcurgere recursiva prin trie
	for (int i = 0; i < 26; i++)
		if (node->children[i]) {
			prefix[length] = i + 'a';
			autocomplete_3(node->children[i], prefix, freq,
						   prev_freq, length + 1);
		}
}

// se apeleaza pt fiecare caz de autocomplete functia corespunzatoare
void autocomplete_prep(trie_t *trie, char *prefix, int num)
{
	trie_node_t *node = trie->root;

	// salvam ultimul nod ce formeaza prefixul dat, dandu-l ca radacina
	// in functiile de autocomplete
	for (int i = 0; i < strlen(prefix); i++) {
		// cazul in care prefixul nu este gasit in trie
		if (!node->children[prefix[i] - 'a']) {
			printf("No words found\n");

			// daca avem autocomplete 0
			if (num == 0) {
				printf("No words found\n");
				printf("No words found\n");
			}
			return;
		}

		node = node->children[prefix[i] - 'a'];
	}

	// copie a prefixului, pierzand prefixul original de la un caz de
	// autocomplete la altul
	char prefix_copy[WORD_SIZE];
	strcpy(prefix_copy, prefix);

	if (num == 1 || num == 0)
		autocomplete_1(node, prefix, strlen(prefix));

	if (num == 2 || num == 0) {
		// punem valoarea originala in prefix
		strcpy(prefix, prefix_copy);

		char shortest[WORD_SIZE]; // aici va fi salvat cuvantul potrivit
		int first = 1;      // cuvantul este primul gasit

		autocomplete_2(node, prefix, shortest, strlen(prefix), &first);
		printf("%s\n", shortest);
	}

	if (num == 3 || num == 0) {
		// punem valoarea originala in prefix
		strcpy(prefix, prefix_copy);

		char freq[WORD_SIZE];    // aici va fi salvat cuvantul gasit
		int prev_freq = 0; // frecventa cuvantului salvat

		autocomplete_3(node, prefix, freq, &prev_freq, strlen(prefix));
		printf("%s\n", freq);
	}
}

int main(void)
{
	trie_t *trie = trie_create(sizeof(char), 26);
	char command[WORD_SIZE]; // comanda citita
	char word[WORD_SIZE];    // cuvantul ce urmeaza sa fie adaugat / autocorrect

	while (1) {
		fscanf(stdin, "%s", command);

		if (strcmp(command, "INSERT") == 0) {
			fscanf(stdin, "%s", word);
			trie_insert(trie, word);
		} else if (strcmp(command, "LOAD") == 0) {
			char filename[WORD_SIZE];
			fscanf(stdin, "%s", filename);

			FILE *in = fopen(filename, "rt");
			DIE(!in, "failed to open file");

			// citim toate cuvintele din fisier
			do {
				fscanf(in, "%s", word);
				trie_insert(trie, word);
			} while (feof(in) == 0);

			trie_remove(trie, word); // ultimul cuvant va fi salvat de 2 ori

			fclose(in);
		} else if (strcmp(command, "AUTOCORRECT") == 0) {
			int num, check = 0;
			fscanf(stdin, "%s%d", word, &num);

			char new[WORD_SIZE]; // rezultatul autocorrectului
			autocorrect(trie->root, new, 0, word, num, &check);

			// verificam daca nu am gasit niciun cuvant
			if (check == 0)
				printf("No words found\n");
		} else if (strcmp(command, "AUTOCOMPLETE") == 0) {
			char prefix[WORD_SIZE];
			int num; // numarul autocompleteului

			fscanf(stdin, "%s%d", prefix, &num);
			autocomplete_prep(trie, prefix, num);
		} else if (strcmp(command, "REMOVE") == 0) {
			fscanf(stdin, "%s", word);
			trie_remove(trie, word);
		} else if (strcmp(command, "EXIT") == 0) {
			trie_free(&trie);
			break;
		}
	}
	return 0;
}

